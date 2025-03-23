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
            writer.write("n;Bn;Un;Cn;Dn;Dn-Cn");
            writer.newLine();
            for (int n = 1000; n <= 100000; n += 1000) {
                for(int k=0;k<50;k++){
                    ArrayList<Integer> bins = new ArrayList<Integer>(Collections.nCopies(n, 0));
                    int momentPierwszejKolizji = -1;
                    int liczbaPustychUrn=n;
                    int liczbaUrn1kula=n;
                    int liczaPustychUrnPoNWrzuceniu=0;
                    int kolekcjonerKuponów=-1;
                    int kolekcjonerKuponówBrat=-1;
                    int i=1;
                    int randomValue=0;
                   while(kolekcjonerKuponówBrat==-1){
                        randomValue = mt.nextInt(n);
                        if (bins.get(randomValue) == 0) {
                            liczbaPustychUrn--; 
                        }
                        if (bins.get(randomValue) > 1 && momentPierwszejKolizji == -1) {
                            momentPierwszejKolizji = i; 
                       
                        }
                        if(bins.get(randomValue) == 1){
                            liczbaUrn1kula--;
                        }
                        bins.set(randomValue, bins.get(randomValue) + 1);
                        if(liczbaPustychUrn==0 && kolekcjonerKuponów==-1){
                         kolekcjonerKuponów=i;
                        }
                        if(liczbaUrn1kula==0 && kolekcjonerKuponówBrat==-1){
                            kolekcjonerKuponówBrat=i;
                           }
                        if(i==n){
                            liczaPustychUrnPoNWrzuceniu=liczbaPustychUrn;
                        }
                        i++;
                    }
int dncn=kolekcjonerKuponówBrat-kolekcjonerKuponów;
writer.write(n+";"+momentPierwszejKolizji+";"+liczaPustychUrnPoNWrzuceniu+";"+kolekcjonerKuponów+";"+kolekcjonerKuponówBrat+";"+dncn);
writer.newLine();

                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static void main(String[] args) {
        MersenneTwister mt = new MersenneTwister();
        calculateBallsAndBins(mt, "wyniki1.txt");
    }
}

