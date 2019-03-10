import java.util.Scanner;
import java.text.DecimalFormat;

public class Main {

    public static void main(String[] args) {
        DecimalFormat df2 = new DecimalFormat("000.00");
        Scanner kb = new Scanner(System.in);
        System.out.println("Please enter an initial guess: ");
        int initialGuess = kb.nextInt();
        System.out.println("your guess is: "+initialGuess);

        Itteration i1 = new Itteration();


        double[][] geometry1 = new double[12][46];
        for(int y=0;y<geometry1.length;y++){
            for(int x=0;x<geometry1[0].length;x++){
                if(x==0 || x==geometry1[0].length-1){
                    geometry1[y][x] = 0;
                }
                else if( y==0 || y==geometry1.length-1){
                    geometry1[y][x] = 0;
                }
                else if(y>=5 && y<=6 && x>=10 && x<=17){
                    geometry1[y][x] = 0;
                }
                else if(y>=5 && y<=6 && x>=28 && x<=35){
                    geometry1[y][x] = 100;
                }
                else{
                    geometry1[y][x] = initialGuess;
                }
            }
        }

        double[][] geometry2 = new double[24][92];
        for(int y=0;y<geometry2.length;y++){
            for(int x=0;x<geometry2[0].length;x++){
                if(x==0 || x==geometry2[0].length-1){
                    geometry2[y][x] = 0;
                }
                else if( y==0 || y==geometry2.length-1){
                    geometry2[y][x] = 0;
                }
                else if(y>=10 && y<=12 && x>=20 && x<=34){
                    geometry2[y][x] = 0;
                }
                else if(y>=10 && y<=12 && x>=56 && x<=70){
                    geometry2[y][x] = 100;
                }
                else{
                    geometry2[y][x] = initialGuess;
                }
            }
        }

        double alpha1 = 1.0;
        double alpha2 = 1.5;
        double alpha3 = 1.7;
        System.out.println("starting");
        i1.Solve(geometry1,alpha1,1.0, initialGuess);
        i1.Solve(geometry1,alpha2,1.0, initialGuess);
        i1.Solve(geometry1,alpha3,1.0, initialGuess);

        i1.Solve(geometry2,alpha1,0.5, initialGuess);
        i1.Solve(geometry2,alpha2,0.5, initialGuess);
        i1.Solve(geometry2,alpha3,0.5, initialGuess);
        System.out.println("done");
    }
}
