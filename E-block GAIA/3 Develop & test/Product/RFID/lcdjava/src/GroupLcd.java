import java.awt.*;
import java.awt.geom.*;
import java.lang.Math;

public class GroupLcd {
	PointLcd[] group8bit = new PointLcd[1000];
	int valuePoint;
	int group;
	int x;
	int y;
	int lensceen;
	
	GroupLcd(int valuePoint,int group,int x,int y,int lensceen){
		this.valuePoint = valuePoint;
		this.group      = group;
		this.x          = x;
		this.y			= y;
		this.lensceen   = lensceen;
		int valuet      = valuePoint;
		
		for(int i=0;i<lensceen;i++){
			if(valuet/Math.pow(2,(lensceen-1)-i) >= 1){
				group8bit[(lensceen-1)-i] = new PointLcd(group,(lensceen-1)-i,x+(i*5),y,1);
				valuet = valuet-(int)Math.pow(2,(lensceen-1)-i);
			}
			else{
			    group8bit[(lensceen-1)-i] = new PointLcd(group,(lensceen-1)-i,x+(i*5),y,0);
			}
		}
	}
	
	public int groupValue(){
		int groupValue = 0;
		for(int i=0;i<lensceen;i++){
			if(group8bit[i].getValue() == 1){
				groupValue = groupValue + (int)Math.pow(2,i);
			}
		}
		return groupValue;
	}
	
	public void inputValueGroup(int group,int bit,int value){
		for(int i=0;i<lensceen;i++){
			if(group8bit[i].checkGB(group, bit)){
				group8bit[i].setValue(value);
			}
		}
	}
	
	public boolean inputValueArea(int x,int y,int value){
		for(int i=0;i<lensceen;i++){
			if(group8bit[i].checkMyArea(x,y)){
				group8bit[i].setValue(value);
				return true;
			}
		}
		return false;
	}
	
	public void GroupInput(int value){
		for(int i=0;i<lensceen;i++){
			if(value/Math.pow(2,(lensceen-1)-i) >= 1){
				group8bit[(lensceen-1)-i] = new PointLcd(group,(lensceen-1)-i,x+(((lensceen-1)-i)*5),y,1);
				value = value-(int)Math.pow(2,(lensceen-1)-i);
			}
			else{
			    group8bit[(lensceen-1)-i] = new PointLcd(group,(lensceen-1)-i,x+(((lensceen-1)-i)*5),y,0);
			}
		}
	}
	
	public boolean checkMyGroup(int x,int y){
		for(int i=0;i<lensceen;i++){
			if(group8bit[i].checkMyArea(x,y)){
				return true;
			}
		}
		return false;
	}
	
	public void drawGroup(Graphics g){
		for(int i=0;i<lensceen;i++){
			group8bit[i].drawPoint(g);
		}
	}	
}
