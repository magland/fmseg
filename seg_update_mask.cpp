#include "seg_update_mask.h"

bool compute_distance_map_kernel(const Array2D &X,Array2D &Y,int x,int y) {
	bool changed=false;
	for (int dy=-1; dy<=1; dy++)
	for (int dx=-1; dx<=1; dx++) {
		if ((!dx)||(!dy)) {
			int x2=x+dx; int y2=y+dy;
			if (Y.getValue(x2,y2)) {
				float dist=qAbs(X.getValue(x,y)-X.getValue(x2,y2));
				float newval=Y.getValue(x2,y2)+dist;
				if (newval) {
					if ((Y.getValue(x,y)==0)||(Y.getValue(x,y)>newval)) {
						Y.setValue(newval,x,y);
						changed=true;
					}
				}
			}
		}
	}
	return changed;
}

Array2D compute_distance_map(const Array2D &X,int x0,int y0,int rad) {
	int N1=X.N1(); int N2=X.N2();
	Array2D Y; Y.allocate(N1,N2);
	Y.setValue(1,x0,y0);
	
	int pass=1;
	int num_changed=1;
	while ((num_changed)&&(pass<100)) {
		num_changed=0;
		for (int y=y0-rad; y<=y0+rad; y++)
		for (int x=x0-rad; x<=x0+rad; x++) {
			bool changed=compute_distance_map_kernel(X,Y,x,y);
			if (changed) num_changed++;
		}
		for (int y=y0+rad; y>=y0-rad; y--)
		for (int x=x0+rad; x>=x0-rad; x--) {
			bool changed=compute_distance_map_kernel(X,Y,x,y);
			if (changed) num_changed++;
		}
		for (int y=y0-rad; y<=y0+rad; y++)
		for (int x=x0+rad; x>=x0-rad; x--) {
			bool changed=compute_distance_map_kernel(X,Y,x,y);
			if (changed) num_changed++;
		}
		for (int y=y0+rad; y>=y0-rad; y--)
		for (int x=x0-rad; x<=x0+rad; x++) {
			bool changed=compute_distance_map_kernel(X,Y,x,y);
			if (changed) num_changed++;
		}
		pass++;
	}
	if (pass>=100) {
		printf("%d passes\n",pass);
	}
	return Y;
}


void seg_update_mask(const Array2D &array,Array2D &mask,QPoint point,float threshold) {
	Array2D DT=compute_distance_map(array,point.x(),point.y(),30);
	for (int y=0; y<mask.N2(); y++)
	for (int x=0; x<mask.N1(); x++) {
		if ((DT.getValue(x,y))&&(DT.getValue(x,y)<=threshold)) {
			mask.setValue(1,x,y);
		}
	}
}
