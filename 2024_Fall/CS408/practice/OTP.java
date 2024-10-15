import java.util.Random;
import java.util.Scanner;

public class OTP {
  private int message;
  private int cipherText;
  private int key;

  public OTP(int message) {
    this.message = message;
    int key_len = Integer.toBinaryString(message).length();

    Random rand = new Random();
    key = 0b0;

    System.out.println(Integer.toBinaryString(message));
    System.out.println(key_len);
    for (int i = 0; i < key_len; i++) {
      key += (rand.nextInt(99999) % 2) * Math.pow(2, i);
    }

    System.out.println(Integer.toBinaryString(key));
    cipherText = message ^ key;
    System.out.print("CT (binary): ");
    System.out.println(Integer.toBinaryString(cipherText));

    System.out.print("CT (text): ");
    System.out.println(binaryToString(Integer.toBinaryString(cipherText)));
  }

  private static String stringToBinary(String msg) {
    StringBuilder result = new StringBuilder();
    char[] characters = msg.toCharArray();

    for (char c : characters) {
      String binary = String.format("%8s", Integer.toBinaryString(c));
      binary = binary.replaceAll(" ", "0");
      result.append(binary);
    }

    return result.toString();
  }

  private static String binaryToString(String binary) {
    StringBuilder result = new StringBuilder();
    String[] binaries = binary.split(" ");

    for (String b : binaries) {
      int charCode = Integer.parseInt(b, 2);
      result.append((char) charCode);
    }

    return result.toString();
  }

  public static void main(String[] args) {
    Scanner ui = new Scanner(System.in);

    System.out.println("Message: ");
    String msg = ui.nextLine();

    msg = stringToBinary(msg);
    System.out.println(msg);
    OTP otp = new OTP(Integer.parseInt(msg, 2));

    ui.close();
  }
}
