import java.awt.*;
import java.io.*;
import java.awt.geom.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;


public class TestAll extends Frame {

	GroupLcd[] ta = new GroupLcd[2000];
	int valuelogic;
	String name;
	int xuse;
	int yuse;
	boolean start;
	int highlcd;
	int lenglcd;
	int totalLcd;
	
	TestAll(String name,int lenglcd,int highlcd){
		this.name = name;
		this.highlcd = highlcd;
		this.lenglcd = lenglcd;
		this.totalLcd = lenglcd*highlcd;
 		start = true;
		int x = 5;
		int y = 30;
		valuelogic = 1;
		
		for(int i=0;i<highlcd;i++){
			
			ta[i] =  new GroupLcd(2,i,x,y,lenglcd);
			y = y+5;
			
		}
		
		setSize((lenglcd*5)+10,(highlcd*5)+35);
		setVisible(true);
		
		addWindowListener(new WindowAdapter() {
	        public void windowClosing(WindowEvent e) {
	           System.exit(0);
	           }
	        }
	    );
		
		addMouseListener(new MouseAdapter(){
			public void mousePressed(MouseEvent e){
				 int i=0;
			     xuse = e.getX();
			     yuse = e.getY();
			     while(!ta[i].inputValueArea(xuse,yuse,valuelogic)){i++;}
			     repaint();
			}			
		});
		
		addMouseMotionListener(new MouseMotionAdapter(){
			public void mouseDragged(MouseEvent e){
				 int i=0;
				 xuse = e.getX();
				 yuse = e.getY();
			     while(!ta[i].inputValueArea(xuse,yuse,valuelogic)){i++;}
			     repaint();
			}
		});
		
		addKeyListener(new KeyAdapter(){
			public void keyPressed(KeyEvent e){
				if(e.getKeyChar()=='b'){
					System.out.println("You Select Black Color");
					valuelogic = 1; 
				}
				if(e.getKeyChar()=='w'){
					System.out.println("You Select White Color");
					valuelogic = 0;
				}
				if(e.getKeyChar()=='s'){
					System.out.println("You Save Data Compelete");
					saveNokiaLcd();
				}
			}
		});
		
		
		
	}
	
	
	public void update(Graphics g){
		for(int i=0;i<highlcd;i++){
			if(ta[i].checkMyGroup(xuse,yuse)){
				ta[i].drawGroup(g);
			}
		}	
	}
	
	public void paint(Graphics g){
		for(int i=0;i<highlcd;i++){
			ta[i].drawGroup(g);
		}
	}
	
	public void saveNokiaLcd(){
		try{
		FileWriter first = new FileWriter(new File("first1.txt"));
		first.write("int[]"+name+" = \n");    //The following lines writes
		first.write("{");
		for(int i=0;i<highlcd;i++){
			String datat = String.valueOf(ta[i].groupValue());
			first.write(datat+", ");
			if(i%lenglcd == 0 && i!=0){
				first.write("\n");
			}
		}
		first.write("\n");
		first.write("};");
        first.close();
		}
		catch(IOException e){
			
		}
		
	}
	
	public static void main(String[]args){
		TestAll a = new TestAll("pictitle",24,50);
		
	}
	
}
