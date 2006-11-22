#ifndef BRECT__H
#define BRECT__H

class BRect
{
public:
		double x1,y1,x2,y2,width,height;
		BRect(double x1_, double y1_, double x2_, double y2_)
		{
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			width = x2-x1;
			height = y2-y1;
		};
};

#endif
