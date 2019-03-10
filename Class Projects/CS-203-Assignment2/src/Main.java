/**
 * Created by Andrew Cicchinelli
 */

import java.util.*;

public class Main {
    public static void main(String[] args) {

        //make array of length 25000
        int[] randomArray = new int[5000];
        int[] randomArray2 = new int[5000];
        int[] randomArray3 = new int[5000];
        Random rand = new Random();
        //populate it with random ints
        for (int i=0;i<randomArray.length;i++)
        {
            randomArray[i] = rand.nextInt(1000) + 1;
        }
        //run it through sorting algorithms
        Search s = new Search();

        for (int i=0;i<randomArray.length;i++)
        {
            randomArray2[i] = randomArray[i];
        }

        for (int i=0;i<randomArray.length;i++)
        {
            randomArray3[i] = randomArray[i];
        }

        long insertStartTime = System.nanoTime();
        String insertionReturn = s.insertionSort(randomArray, false);
        long insertEndTime = System.nanoTime();
        long insertElapsedTime = insertEndTime - insertStartTime;
        System.out.println("Insertion Sort:\n" +
                insertionReturn + '\n' +
                "Completed in: " + insertElapsedTime + " nano seconds");

        long quickStartTime = System.nanoTime();
        String quickReturn = s.quickSort(randomArray2, false);
        long quickEndTime = System.nanoTime();
        long quickElapsedTime = quickEndTime - quickStartTime;
        System.out.println("Quick Sort:\n" +
                quickReturn + '\n' +
                "Completed in: " + quickElapsedTime + " nano seconds");

        long mergeStartTime = System.nanoTime();
        String mergeReturn = s.mergeSort(randomArray3, false);
        long mergeEndTime = System.nanoTime();
        long mergeElapsedTime = mergeEndTime - mergeStartTime;
        System.out.println("Merge Sort:\n" +
                mergeReturn + '\n' +
                "Completed in: " + mergeElapsedTime + " nano seconds");

        int[] testArray = new int[] {10, 15, 2, 6, 20, 5};
        int[] testArray2 = new int[] {10, 15, 2, 6, 20, 5};
        int[] testArray3 = new int[] {10, 15, 2, 6, 20, 5};

        s.printArray(testArray);
        System.out.println(s.insertionSort(testArray, true));

        s.printArray(testArray2);
        System.out.println(s.quickSort(testArray2, true));

        s.printArray(testArray3);
        System.out.println(s.mergeSort(testArray3, true));

        System.out.println("All Sorting Complete.");

    }
}
