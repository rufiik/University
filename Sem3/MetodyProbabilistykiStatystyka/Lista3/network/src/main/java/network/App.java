package network;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import org.apache.commons.math3.random.MersenneTwister;
import java.util.ArrayList;
import java.util.Collections;

public class App {
    public static void networkComunication(ArrayList<Integer> list, BufferedWriter writer, MersenneTwister mt, double p) throws IOException {
        int min=1000000;
        int n = list.size();
        int i = 1;
        while(Collections.frequency(list, 1) != list.size()){
        for(int j=0; j<n; j++){
            if(list.get(j)==-1 && mt.nextDouble()<=p){
                list.set(j, 1);
            }
        
        }
        i++;
    }
    min = i;
    writer.write(n + ";" + min + ";" + p);
    writer.newLine();


    }
    public static void generateList(MersenneTwister mt, String fileName, double p) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, false))) {
            writer.write("n;min;p");
            writer.newLine();
            for (int n = 10000; n <= 1000000; n += 10000) {
                for (int k = 0; k < 50; k++) {
                    ArrayList<Integer> list = new ArrayList<>(Collections.nCopies(n, -1));
                    networkComunication(list, writer,mt,p);


                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        MersenneTwister mt = new MersenneTwister();
        generateList(mt, "wyniki.txt",0.1);
        generateList(mt, "wyniki2.txt", 0.5);
    }
}