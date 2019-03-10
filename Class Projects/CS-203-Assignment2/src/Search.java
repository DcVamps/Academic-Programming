/**
 * Created by Andrew Cicchinelli
 */

import java.util.*;

public class Search {

    int quickSortComp=0, quickSortSwap=0, mergeSortComp=0, mergeSortSwap=0;

    public String insertionSort(int[] arr, boolean print)
    {
        int temp;
        int comparisons = 0, swaps = 0;
        boolean swap = true;
        for (int i = 1; i < arr.length; i++) {
            for(int j = i ; j > 0 ; j--){

                if(arr[j] < arr[j-1]){
                    comparisons++;
                    if(swap)
                    {
                        swaps++;
                        swap = false;
                    }
                    temp = arr[j];
                    arr[j] = arr[j-1];
                    arr[j-1] = temp;
                }
                swap = true;
            }
        }
        if (print){
            printArray(arr);
        }
        return "Number of Comparisons: " + comparisons +
                "\nNumber of Swaps: " + swaps;
    }

    public String quickSort(int[] arr, boolean print)
    {
        //call sorting algorithm that modifies the class variables
        this.sortQ(arr, 0, arr.length-1);
        String returntext = "Number of Comparisons: " + this.quickSortComp +
                "\nNumber of Swaps: " + this.quickSortSwap;
        this.quickSortSwap = 0;
        this.quickSortComp = 0;
        if (print){
            printArray(arr);
        }
        return returntext;
    }

    private int partition(int arr[], int low, int high) {
        int pivot = arr[high];
        int i = (low - 1); // index of smaller element
        for (int j = low; j < high; j++)
        {
            // If current element is smaller than or
            // equal to pivot
            if (arr[j] <= pivot)
            {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                this.quickSortSwap++;
            }
        }
        int temp = arr[i+1];
        arr[i+1] = arr[high];
        arr[high] = temp;

        return i+1;
    }

    private  void sortQ(int arr[], int low, int high)
    {
        if (low < high)
        {
            //pi is partitioning index, arr[pi] is
            //now at right place
            int pi = partition(arr, low, high);

            // Recursively sort elements before
            // partition and after partition
            sortQ(arr, low, pi-1);
            this.quickSortComp++;
            sortQ(arr, pi+1, high);
            this.quickSortComp++;
        }
    }

    public String mergeSort(int[] arr, boolean print)
    {
        //call sorting algorithm that modifies the class variables
        sortM(arr, 0, arr.length-1);
        String returntext = "Number of Comparisons: " + this.mergeSortComp +
                "\nNumber of Swaps: " + this.mergeSortSwap;
        this.mergeSortSwap = 0;
        this.mergeSortComp = 0;
        if (print){
            printArray(arr);
        }
        return returntext;
    }

    private void merge(int arr[], int l, int m, int r)
    {
        // Find sizes of two subarrays to be merged
        int n1 = m - l + 1;
        int n2 = r - m;
        //Create temp arrays
        int L[] = new int [n1];
        int R[] = new int [n2];

        //Copy data to temp arrays
        for (int i=0; i<n1; ++i)
            L[i] = arr[l + i];
        for (int j=0; j<n2; ++j)
            R[j] = arr[m + 1+ j];
        // Initial indexes of first and second subarrays
        int i = 0, j = 0;
        // Initial index of merged subarry array
        int k = l;
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j])
            {
                arr[k] = L[i];
                i++;
                this.mergeSortSwap++;
            }
            else
            {
                arr[k] = R[j];
                j++;
                this.mergeSortSwap++;
            }
            k++;
        }
        /* Copy remaining elements of L[] if any */
        while (i < n1)
        {
            arr[k] = L[i];
            i++;
            k++;
        }
        /* Copy remaining elements of R[] if any */
        while (j < n2)
        {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    void sortM(int arr[], int l, int r)
    {
        if (l < r)
        {
            // Find the middle point
            int m = (l+r)/2;

            // Sort first and second halves
            sortM(arr, l, m);
            sortM(arr , m+1, r);

            // Merge the sorted halves
            merge(arr, l, m, r);
            this.mergeSortComp++;
            this.mergeSortComp++;
        }
    }

    public void printArray(int[] arr)
    {
        String out = "";
        for (int i=0; i<arr.length;i++)
        {
            out = out + arr[i]+',';
        }
        System.out.println(out);
    }
}