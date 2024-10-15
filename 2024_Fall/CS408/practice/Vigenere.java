import java.util.Scanner;

public class Vigenere {

  private String key;
  private String CT;
  private String PT;
  private static Scanner scan = new Scanner(System.in);

  public Vigenere(String key, String PT) {
    this.key = key;
    this.PT = PT;
  }

  public void setKey(String newKey) {
    key = newKey;
  }

  public void setPT(String newPT) {
    PT = newPT;
  }

  public void setCT(String newCT) {
    CT = newCT;
  }

  public void encrypt() {
    setPT(PT.toUpperCase());
    setKey(key.toUpperCase());

    System.out.println(key.charAt(0) - 0);

    // 65 to 90

    char[] ptLetters = PT.toCharArray();
    for (int i = 0; i < ptLetters.length; i++) {
      int shifted = ((ptLetters[i] + key.charAt(i%key.length())) % 26) + 65;
      ptLetters[i] = (char)shifted;
      // System.out.print((char)(ptLetters[i] + key.charAt(i % key.length()) % 26 + 65));
      // System.out.print((ptLetters[i]-0));
    }

    setCT(String.valueOf(ptLetters));

    System.out.println("PT: "+PT);
    System.out.println("CT: "+CT);
  }

  public static void main(String[] args) {
    System.out.println("Vigenere Cipher\n");
    System.out.print("Key:\t");
    String key = scan.nextLine();

    System.out.print("Message:\t");
    String PT = scan.nextLine();

    Vigenere cipher = new Vigenere(key, PT);
    //Vigenere cipher = new Vigenere("zack", "Burhanuddin");

    cipher.encrypt();
  }
}
