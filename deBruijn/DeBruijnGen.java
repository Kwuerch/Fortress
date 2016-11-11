public class DeBruijnGen{
   private final long DEBRUIJN = 0x03f79d71b4cb0a89L;
   public void getList(){
      Graph grp = new Graph(true, 64);

      for(int i = 0; i < 64; i++){
         grp.insertEdge(i, ((i << 1) + 1) & 0x3f);
         grp.insertEdge(i, (i << 1) & 0x3f);
      }

      grp.printGraph();
   }

   public String genLookupTable(){
      String res = "";
      int[] arr = new int[64];
      long num  = 1;
      for(int i = 0; i < 64; i++, num <<= 1){
         arr[(int)((((num) * DEBRUIJN) >>> 58) & 0x3f)] = i;
      }

      for(int i = 1; i <= 64; i++){
         res += arr[i-1] + (i % 8 == 0 ? ",\n" : ", "); 
      }

      return res;
   }
}
