import java.util.*;

public class ISBNSort implements Comparator <Book>
{
   public int compare(final Book o1, final Book o2)
   {
   
   return o1.getISBN().compareTo(o2.getISBN());
   }//end method
}//end class