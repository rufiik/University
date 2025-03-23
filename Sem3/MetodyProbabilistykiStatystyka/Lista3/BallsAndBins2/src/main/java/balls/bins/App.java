package balls.bins;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import org.apache.commons.math3.random.MersenneTwister;
import java.util.ArrayList;
import java.util.Collections;
public class App {
 public static void calculateBallsAndBins(MersenneTwister mt, String fileName) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, false))) {
            writer.write("n;max");
            writer.newLine();
            for (int n = 10000; n <= 1000000; n += 10000) {
                for(int k=0;k<50;k++){
                    ArrayList<Integer> bins = new ArrayList<Integer>(Collections.nCopies(n, 0));
                    int i=0;
                    int randomValue=0;
                    int max = -1;
                   while(i!=n){
                        randomValue = mt.nextInt(n);
                        bins.set(randomValue, bins.get(randomValue) + 1);
                        i++;
                    }
        
max = Collections.max(bins);                   
writer.write(n+";"+max);
writer.newLine();

                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }



    public static void calculateBallsAndBins2(MersenneTwister mt, String fileName) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, false))) {
            writer.write("n;max");
            writer.newLine();
            for (int n = 10000; n <= 1000000; n += 10000) {
                for(int k=0;k<50;k++){
                    ArrayList<Integer> bins = new ArrayList<Integer>(Collections.nCopies(n, 0));
                    int i=0;
                    int randomValue=0;
                    int randomValue1=0;
                    int randomValue2=0;
                    int max = -1;
                   while(i!=n){
                        randomValue1 = mt.nextInt(n);
                        randomValue2 = mt.nextInt(n);
                        if(bins.get(randomValue1)>= bins.get(randomValue2)){
                            randomValue=randomValue2;
                        }
                        else{
                            randomValue=randomValue1;
                        }
                        bins.set(randomValue, bins.get(randomValue) + 1);          
                        i++;
                    }
                    max = Collections.max(bins);                    
writer.write(n+";"+max);
writer.newLine();

                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }   
    public static void main(String[] args) {
        MersenneTwister mt = new MersenneTwister();
        calculateBallsAndBins(mt, "wynikimax1.txt");
        calculateBallsAndBins2(mt, "wynikimax2.txt");
    }
}

