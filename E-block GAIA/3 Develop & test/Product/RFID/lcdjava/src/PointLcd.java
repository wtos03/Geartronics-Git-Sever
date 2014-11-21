import java.awt.*;
import java.awt.geom.*;

public class PointLcd {
	
	private Rectangle2D r;
	private Graphics2D g2;
	private int group;
	private int bit;
	private int value;
	
	PointLcd(int group,int bit,int x,int y,int value){
		this.group = group;
		this.bit   = bit;
		this.value = value;
		this.r = new Rectangle2D.Double(x,y,5,5);
	}
	
	public void setValue(int value){
		this.value = value;
	}
	
	public int getValue(){
		return value;
	}
	
	public boolean checkGB(int group,int bit){
		if(this.group == group && this.bit == bit){
			return true;
		}
		return false;
	}
	
	public boolean checkMyArea(int x,int y){
		return r.contains(x,y);
	}
	
	public void drawPoint(Graphics g){
		this.g2 = (Graphics2D)g;			
		if(value == 0){
			g2.setPaint(Color.WHITE);
            g2.fill(r);
            
            g2.setPaint(Color.GRAY);
            g2.draw(r);
		}
		
		else{
			g2.setPaint(Color.BLACK);
            g2.fill(r);
		}
	}
	
}
