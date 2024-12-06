import java.io.IOException;
import java.util.StringTokenizer;
import java.util.HashSet;
import java.util.Set;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.BooleanWritable;
import org.apache.hadoop.io.NullWritable;

public class FindMissingCardsHBase {

  private static final String TABLE_NAME = "missing_cards";
  private static final String COLUMN_FAMILY = "card_info";
  private static final String COLUMN_QUALIFIER = "present";

  private static final int SUIT = 0;
  private static final int RANK = 1;
  public static String[] suits = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  public static String[] ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "JACK", "QUEEN", "KING", "ACE" };

  private static Configuration getHBaseConfiguration() {
    Configuration config = HBaseConfiguration.create();
    config.set("hbase.zookeeper.quorum", "localhost"); // Update with your Zookeeper quorum
    config.set("hbase.zookeeper.property.clientPort", "2181");
    return config;
  }

  // Create HBase table
  private static void createHBaseTable() throws IOException {
    Configuration config = getHBaseConfiguration();
    try (Connection connection = ConnectionFactory.createConnection(config);
        Admin admin = connection.getAdmin()) {

      TableName tableName = TableName.valueOf(TABLE_NAME);
      if (!admin.tableExists(tableName)) {
        HTableDescriptor tableDescriptor = new HTableDescriptor(tableName);
        HColumnDescriptor columnDescriptor = new HColumnDescriptor(COLUMN_FAMILY);
        tableDescriptor.addFamily(columnDescriptor);
        admin.createTable(tableDescriptor);
        System.out.println("Table created: " + TABLE_NAME);
      } else {
        System.out.println("Table already exists: " + TABLE_NAME);
      }
    } catch (IOException e) {
      System.err.println("Error creating HBase table: " + e.getMessage());
      throw e;
    }
  }

  public static class DeckMapper extends Mapper<Object, Text, Text, BooleanWritable> {
    private static Set<String> inputDeck = new HashSet<>();

    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
      String card = value.toString().trim();
      context.write(new Text(card), new BooleanWritable(true));
    }
  }

  public static class DeckReducer extends Reducer<Text, BooleanWritable, Text, BooleanWritable> {
    private Connection connection;
    private Table table;
    private int recordCount = 0;

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
      try {
        Configuration config = getHBaseConfiguration();
        connection = ConnectionFactory.createConnection(config);
        table = connection.getTable(TableName.valueOf(TABLE_NAME));
        System.out.println("HBase connection established in Reducer");
      } catch (IOException e) {
        System.err.println("Error setting up HBase connection in Reducer: " + e.getMessage());
        throw e;
      }
    }

    private static Set<String> initializeDeck() {
      Set<String> deck = new HashSet<>();
      for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
          deck.add(ranks[j] + " " + suits[i]);
      return deck;
    }

    private static Set<String> fullDeck = initializeDeck();

    public void reduce(Text key, Iterable<BooleanWritable> values, Context context)
        throws IOException, InterruptedException {
      fullDeck.remove(key.toString());
    }

    @Override
    public void cleanup(Context context) throws IOException, InterruptedException {
      try {
        // Store missing cards in HBase
        for (String card : fullDeck) {
          try {
            Put put = new Put(Bytes.toBytes(card));
            put.addColumn(
                Bytes.toBytes(COLUMN_FAMILY),
                Bytes.toBytes(COLUMN_QUALIFIER),
                Bytes.toBytes("missing"));
            table.put(put);
            recordCount++;

            // Also write to HDFS output
            context.write(new Text(card), new BooleanWritable(false));
          } catch (IOException e) {
            System.err.println("Error putting record to HBase: " + e.getMessage());
            throw e;
          }
        }

        // Write the record count
        context.write(new Text("Total missing cards: " + recordCount), null);
      } finally {
        // Always close resources
        if (table != null)
          table.close();
        if (connection != null)
          connection.close();
      }
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = getHBaseConfiguration();
    Job job = Job.getInstance(conf, "missing cards hbase");
    job.setJarByClass(FindMissingCardsHBase.class);
    job.setMapperClass(DeckMapper.class);
    job.setReducerClass(DeckReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(BooleanWritable.class);

    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
