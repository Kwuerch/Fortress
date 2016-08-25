public class DeBruijnGen{
   public void getList(){
      Graph grp = new Graph(true, 64);

      for(int i = 0; i < 64; i++){
         grp.insertEdge(i, ((i << 1) + 1) & 0x3f);
         grp.insertEdge(i, (i << 1) & 0x3f);
      }

      grp.printGraph();
   }
}
