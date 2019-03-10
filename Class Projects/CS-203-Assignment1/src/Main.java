import java.util.*;
import java.io.*;

public class Main {

    public static void main(String[] args) {
        Scanner fs = new Scanner("Input.txt");
        Scanner ks = new Scanner(System.in);
        Node n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, firstNode, secondNode;

        while (fs.hasNextLine()) {
            String graph = fs.nextLine();
            //loop through input to build the graph based on the input data
            int vert = Integer.parseInt(graph.split("\\(")[0].replace(" ", ""));
            //Create the graph to be looped though
            Graph g = new Graph();

            for (int i = 1; i <= vert; i++) {
                switch (i) {
                    case 1:
                        n1 = new Node(i);
                        g.addNode(n1);
                        break;
                    case 2:
                        n2 = new Node(i);
                        g.addNode(n2);
                        break;
                    case 3:
                        n3 = new Node(i);
                        g.addNode(n3);
                        break;
                    case 4:
                        n4 = new Node(i);
                        g.addNode(n4);
                        break;
                    case 5:
                        n5 = new Node(i);
                        g.addNode(n5);
                        break;
                    case 6:
                        n6 = new Node(i);
                        g.addNode(n6);
                        break;
                    case 7:
                        n7 = new Node(i);
                        g.addNode(n7);
                        break;
                    case 8:
                        n8 = new Node(i);
                        g.addNode(n8);
                        break;
                    case 9:
                        n9 = new Node(i);
                        g.addNode(n9);
                        break;
                    case 10:
                        n10 = new Node(i);
                        g.addNode(n10);
                        break;
                }
            }

            for (int i = 1; i < graph.split("\\(").length; i++) {

                int first = Integer.parseInt(graph.split("\\(")[i].substring(0, graph.split("\\(")[i].indexOf(",")));
                switch (first) {
                    case 1:
                        firstNode = n1;
                        break;
                    case 2:
                        firstNode = n2;
                        break;
                    case 3:
                        firstNode = n3;
                        break;
                    case 4:
                        firstNode = n4;
                        break;
                    case 5:
                        firstNode = n5;
                        break;
                    case 6:
                        firstNode = n6;
                        break;
                    case 7:
                        firstNode = n7;
                        break;
                    case 8:
                        firstNode = n8;
                        break;
                    case 9:
                        firstNode = n9;
                        break;
                    case 10:
                        firstNode = n10;
                        break;
                }
                int second = Integer.parseInt(graph.split("\\(")[i].substring(graph.split("\\(")[i].indexOf(",") + 1, graph.split("\\(")[i].indexOf(")")));
                switch (second) {
                    case 1:
                        secondNode = n1;
                        break;
                    case 2:
                        secondNode = n2;
                        break;
                    case 3:
                        secondNode = n3;
                        break;
                    case 4:
                        secondNode = n4;
                        break;
                    case 5:
                        secondNode = n5;
                        break;
                    case 6:
                        secondNode = n6;
                        break;
                    case 7:
                        secondNode = n7;
                        break;
                    case 8:
                        secondNode = n8;
                        break;
                    case 9:
                        secondNode = n9;
                        break;
                    case 10:
                        secondNode = n10;
                        break;
                }
                g.connectNode(firstNode, secondNode);
            }
            //traverse the graph and print the data
            g.dfs();
            //Continue to next graph cancel
            System.out.println("Press 1 to precede to the next Graph" +
                    "\nPress 2 to exit");
            if (ks.nextInt() == 2) {
                return;
            }
        }
        System.out.println("Good Bye.");
    }
}

