import java.util.Collections;
import java.util.Scanner;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Deque;
import java.util.ArrayDeque;
import java.io.FileNotFoundException;
import java.io.FileInputStream;
import java.io.File;

/**
 * Graph
 *
 * A class used to represent a graph
 *
 * @author Kyle Wuerch
 * @version Program7 
 */
public class Graph{
   private List<Integer>[] adjacencyList;
   private boolean directed;

   public Graph(boolean directed, int numVerts){
      this.directed = directed;

      adjacencyList = new List[numVerts];
      for(int i = 0; i < numVerts; i++){
         adjacencyList[i] = new LinkedList<Integer>();
      }
   }

   public void insertEdge(int x, int y){
      if(directed){
         adjacencyList[x].add(y);
      }else{
         // Undirected edges must be added in two places
         adjacencyList[x].add(y);
         adjacencyList[y].add(x);
      }

   }

   public void removeEdge(int x, int y){
      if(directed){
         adjacencyList[x].remove(y);
      }else{
         // Undirected edges msut be remove from two places
         adjacencyList[x].remove(y);
         adjacencyList[y].remove(x);
      }
   }

   public List<Integer>[] getAdjacencyList(){
      return adjacencyList;
   }

   public boolean isDirected(){
      return directed;
   }

   public int getNumVertices(){
      return adjacencyList.length;
   }

   public void printGraph(){
      int i = 0;
      for(List<Integer> l : adjacencyList){
         System.out.print(i++ + ": ");
         for(Integer k : l){
            System.out.print(k + " "); 
         }
         System.out.println();
      }
   }
}
