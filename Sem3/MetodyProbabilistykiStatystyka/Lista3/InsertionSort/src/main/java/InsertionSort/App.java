package InsertionSort;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import org.apache.commons.math3.random.MersenneTwister;
import java.util.ArrayList;

public class App {
    public static void insertionSort(ArrayList<Integer> list, BufferedWriter writer) throws IOException {
        int comparisonCount = 0; 
        int swapCount = 0;
        int n = list.size();
        for (int i = 1; i < n; i++) {
            int key = list.get(i);
            int j = i - 1;

            while (j >= 0) {
                comparisonCount++; 
                if (list.get(j) > key) {
                    list.set(j + 1, list.get(j));
                    swapCount++;
                    j--;
                } else {
                    break;
                }
            }

            list.set(j + 1, key);
            if (j + 1 != i) {
                swapCount++; 
            }
        }
        writer.write(n + ";" + comparisonCount + ";" + swapCount);
        writer.newLine();
    }

    public static void generatePermutation(MersenneTwister mt, ArrayList<Integer> list, String fileName) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, false))) {
            writer.write("n;comparisonCount;swapCount");
            writer.newLine();
            for (int n = 100; n <= 10000; n += 100) {
                for (int k = 0; k < 50; k++) {
                    list.clear();
                    for (int i = 1; i <= n; i++) {
                        list.add(i);
                    }
                    ArrayList<Integer> shuffledList = new ArrayList<>();
                    while (!list.isEmpty()) {
                        int index = mt.nextInt(list.size());
                        shuffledList.add(list.remove(index));
                    }
                    insertionSort(shuffledList, writer);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        MersenneTwister mt = new MersenneTwister();
        ArrayList<Integer> list = new ArrayList<>();
        generatePermutation(mt, list, "wyniki.txt");
    }
}