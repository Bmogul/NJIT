import java.io.IOException;
import java.util.StringTokenizer;
import java.util.HashSet;
import java.util.Set;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.BooleanWritable;
import org.apache.hadoop.io.NullWritable;

public class FindMissingCards {

  private static final int SUIT = 0;
  private static final int RANK = 1;
  public static String[] suits = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  public static String[] ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "JACK", "QUEEN", "KING", "ACE" };

  public static class DeckMapper extends Mapper<Object, Text, Text, BooleanWritable> {

    private static Set<String> inputDeck = new HashSet<>();

    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
      String card = value.toString().trim();
      context.write(new Text(card), new BooleanWritable(true));
    }
  }

  public static class DeckReducer extends Reducer<Text, BooleanWritable, Text, BooleanWritable> {
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

    public void cleanup(Context context) throws IOException, InterruptedException {
      for (String card : fullDeck)
        context.write(new Text(card), new BooleanWritable(false));
    }
  }

  public static void main(String[] args) throws Exception{
    System.out.println("Map Reduce find missing cards");
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "missing cards");
    job.setJarByClass(FindMissingCards.class);
    job.setMapperClass(DeckMapper.class);
    job.setReducerClass(DeckReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(BooleanWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
