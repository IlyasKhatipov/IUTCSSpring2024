import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int N = scanner.nextInt();
        char[] alphabet = new char[N];
        for (int i = 0; i < N; ++i) {
            alphabet[i] = scanner.next().charAt(0);
        }
        int M = scanner.nextInt();
        int K = scanner.nextInt();
        int L = scanner.nextInt();

        List<String> l1_strings = new ArrayList<>();
        List<String> l2_strings = new ArrayList<>();
        List<String> l3_strings = new ArrayList<>();
        l1_strings.add("_");
        generateStrings(alphabet, M, K, L, l1_strings, l2_strings, l3_strings);

        for (String str : l1_strings) {
            System.out.print(str + " ");
        }
        System.out.println();
        for (String str : l2_strings) {
            System.out.print(str + " ");
        }
        System.out.println();
        if (l3_strings.get(0).equals("")) {
            l3_strings.clear();
            l3_strings.add("_");
        }
        for (String str : l3_strings) {
            System.out.print(str + " ");
        }
        System.out.println();
    }

    private static void generateStrings(char[] alphabet, int M, int K, int L,
                                        List<String> l1_strings, List<String> l2_strings, List<String> l3_strings) {
        Queue<String> queue = new LinkedList<>();
        queue.add("");

        while ((l1_strings.size() < M || l2_strings.size() < K || l3_strings.size() < L) && !queue.isEmpty()) {
            String prefix = queue.poll();

            if (l1Check(alphabet, prefix) && l1_strings.size() < M) {
                l1_strings.add(prefix);
            }
            if (l2Check(alphabet, prefix) && l2_strings.size() < K) {
                l2_strings.add(prefix);
            }
            if (l3Check(alphabet, prefix) && l3_strings.size() < L) {
                l3_strings.add(prefix);
            }

            for (char c : alphabet) {
                String new_prefix = prefix + c;
                queue.add(new_prefix);
            }
        }

        Collections.sort(l1_strings, (a, b) -> {
            if (a.length() == b.length()) {
                return a.compareTo(b);
            }
            return Integer.compare(a.length(), b.length());
        });
        Collections.sort(l2_strings, (a, b) -> {
            if (a.length() == b.length()) {
                return a.compareTo(b);
            }
            return Integer.compare(a.length(), b.length());
        });
        Collections.sort(l3_strings, (a, b) -> {
            if (a.length() == b.length()) {
                return a.compareTo(b);
            }
            return Integer.compare(a.length(), b.length());
        });
    }

    private static boolean l1Check(char[] alphabet, String s) {
        if (s.length() == 0) {
            return false;
        }
        for (char c : alphabet) {
            if (countChar(s, c) % 2 != 0) {
                return false;
            }
        }
        return true;
    }

    private static boolean l2Check(char[] alphabet, String s) {
        Set<Character> symbols = new HashSet<>();
        for (char c : s.toCharArray()) {
            symbols.add(c);
        }
        return symbols.size() == alphabet.length;
    }

    private static boolean l3Check(char[] alphabet, String s) {
        Set<Character> symbols = new HashSet<>();
        for (char c : s.toCharArray()) {
            symbols.add(c);
        }
        return alphabet.length - symbols.size() == 1;
    }

    private static int countChar(String str, char c) {
        int count = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == c) {
                count++;
            }
        }
        return count;
    }
}
