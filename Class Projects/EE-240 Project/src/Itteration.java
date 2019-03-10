import java.io.*;
import java.text.DecimalFormat;

public class Itteration {

    private boolean loop = false;
    private double G;
    private double maxError;
    private double meshLength;
    private double[][] geometry, previousIterations;
    private int iterationsCount = 0;
    private int maxErrorX;
    private int maxErrorY;
    private int initialGuess;
    private static DecimalFormat df2 = new DecimalFormat("000.00");


    public void Solve(double[][] geometry, double alpha, double meshLength, int initialGuess){
        this.G = alpha/4;
        this.meshLength = meshLength;
        this.initialGuess = initialGuess;
        setPrivateVariables(geometry);
        boolean repeat = true;
        while(repeat){
            this.iterationsCount++;
            repeat = iteration();
        }
        printResults(alpha);
    }
    private boolean iteration(){
        this.maxError = 0;
        double error;
        this.loop = false;
        for (int y=0;y<this.geometry.length-1;y++) {
            for (int x = 0; x < this.geometry[0].length-1; x++) {
                if (this.geometry[y][x] != 0 && this.geometry[y][x] != 100) {
                    this.geometry[y][x] = this.previousIterations[y][x] + (G * (this.geometry[y-1][x] + this.geometry[y][x-1] + this.previousIterations[y+1][x] + this.previousIterations[y][x+1] - (4 * this.previousIterations[y][x])));
                    error = this.previousIterations[y][x] - this.geometry[y][x];
                    if (error > this.maxError) {
                        this.maxError = error;
                        this.maxErrorX = x;
                        this.maxErrorY = y;
                    }

                }
            }
        }
        if(this.maxError > 0.01){
            this.loop = true;
        }
        setPrevious();
        return loop;
    }

    private void setPrivateVariables(double[][] geometry) {
        this.geometry = new double[geometry.length][geometry[0].length];
        for (int y = 0; y < geometry.length; y++) {
            for (int x = 0; x < geometry[0].length; x++) {
                this.geometry[y][x] = geometry[y][x];
            }
        }
        this.previousIterations = new double[geometry.length][geometry[0].length];
        for (int y = 0; y < geometry.length; y++) {
            for (int x = 0; x < geometry[0].length; x++) {
                this.previousIterations[y][x] = geometry[y][x];
            }
        }
    }
    private void setPrevious(){
        for (int y = 0; y < this.geometry.length; y++) {
            for (int x = 0; x < this.geometry[0].length; x++) {
                this.previousIterations[y][x] = this.geometry[y][x];
            }
        }
    }
    private void printResults(double alpha){
        String resultsFileName = "Results_"+this.meshLength+"mm_alpha-"+alpha+".csv";
        File outFile = new File(resultsFileName);
        try {
            PrintWriter out = new PrintWriter(outFile);
            out.println("=== Alpha = " + alpha + " ===\n" +
                    "=== Iteration Count = " + this.iterationsCount + "===\n" +
                    "=== Mesh Length = " + this.meshLength + "mm ===\n" +
                    "=== Initial Guess = " + this.initialGuess + "===\n" +
                    "=== Max Error = " + this.maxError + " at location (" + maxErrorX + " . " + maxErrorY + ") ===");
            String outLine = "";
            for (int y = 0; y < this.geometry.length; y++) {
                for (int x = 0; x < this.geometry[0].length; x++) {
                    outLine = outLine + df2.format(this.geometry[y][x]) + ',';
                }
                out.println(outLine);
                outLine = "";
            }
            out.close();
            System.out.println("printed: " + resultsFileName);
        } catch(FileNotFoundException e){
            e.printStackTrace();
        }
    }
}
