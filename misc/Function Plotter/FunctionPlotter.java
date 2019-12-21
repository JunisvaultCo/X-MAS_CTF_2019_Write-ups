/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package functionplotter;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.*;
import javax.imageio.ImageIO;
import java.util.*;
import java.io.*;
/**
 *
 * @author Junisvault
 */
public class FunctionPlotter {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
      File fin = new File("qr.txt");
      try
      {
        Scanner scann= new Scanner(fin);
        //29 instead of 31 so as to crop properly
        BufferedImage bi = new BufferedImage(29,29,BufferedImage.TYPE_INT_RGB);
        Graphics G= bi.getGraphics();
        int a=0;
        while(scann.hasNextLine())
        {
          String st = scann.nextLine();
          if(a==0)
          {
            a++;
            continue;
          }
          for(int i=1;i<st.length()-1;i++)
          {
            if(st.charAt(i)==' ')continue;
            if(st.charAt(i)=='#')
              G.setColor(Color.BLACK);
            else
              G.setColor(Color.WHITE);
            G.fillRect(i-1,a-1, 1, 1);
          }
          a++;
        }
        ImageIO.write(bi, "png",new File("file.png"));
      }
      catch(Exception e){}
    }
    
}
