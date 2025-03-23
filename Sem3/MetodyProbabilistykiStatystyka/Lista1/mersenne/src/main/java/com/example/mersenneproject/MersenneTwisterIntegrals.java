package com.example.mersenneproject;
import org.apache.commons.math3.random.MersenneTwister;


public class MersenneTwisterIntegrals {
    public static void main(String[] args) {
        MersenneTwister mt = new MersenneTwister();
        int k = 5;
        Random.calculateAndWriteIntegrals(mt, 0,8, 2, k,1, "wyniki1.txt");
        Random.calculateAndWriteIntegrals(mt, 0, 8, 2, 50,1, "wyniki2.txt");
        Random.calculateAndWriteIntegrals(mt, 0, Math.PI, 1, k,2, "wyniki3.txt");
        Random.calculateAndWriteIntegrals(mt, 0, Math.PI, 1, 50,2, "wyniki4.txt");
        Random.calculateAndWriteIntegrals(mt, 0, 1, 27.0/64.0, k,3, "wyniki5.txt");
        Random.calculateAndWriteIntegrals(mt, 0, 1, 27.0/64.0, 50,3, "wyniki6.txt");
        Random.calculateAndWriteIntegrals(mt, -1, 1, 1, k,4, "wyniki7.txt");
        Random.calculateAndWriteIntegrals(mt, -1, 1, 1, 50,4, "wyniki8.txt");

    }




}
