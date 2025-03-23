package com.example.mersenneproject;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import org.apache.commons.math3.random.MersenneTwister;

public class Random {
    public static void calculateAndWriteIntegrals(MersenneTwister mt, double a, double b, double max, int k,int FunctionType, String fileName) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, true))) {
            writer.write("n;wynik;");
            writer.newLine();
            for (int n = 50; n <= 5000; n += 50) {
                for (int j = 0; j < k; j++) {
                    int C = 0;
                    for (int i = n; i > 0; i--) {
                        double randomValueX = mt.nextDouble() * b;
                        double randomValueY = mt.nextDouble() * max;
                        double function=0;
                        switch(FunctionType){
                            case 1:
                                function=Math.cbrt(randomValueX);
                                break;
                            case 2:
                                function=Math.sin(randomValueX);
                                break;
                            case 3:
                                function=4*randomValueX*(Math.pow((1-randomValueX),3));
                                break;
                            case 4:
                                function=0;
                                break;
                        }
                        if(FunctionType==4){
                            if(randomValueX*randomValueX+randomValueY*randomValueY<=1){
                                C++;
                            }
                        }
                        else{
                        if (randomValueY <= function) {
                            C++;
                        }
                    }
                    }
                    writer.write(n+ ";");
                    double result = (double) C / n;
                    double integral = result * (b - a) * max;
                    if(FunctionType==4){
                        integral=4*result;
                    }
                    writer.write(String.valueOf(integral));
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}