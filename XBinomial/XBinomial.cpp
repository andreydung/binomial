#include <math.h>

double __stdcall XBinomial(bool &iscall, double &K, double &S, double &T, double &r, double &sigma, double &delta)
{
	int N=300;
	double h=T/N; // time interval
	double u=exp(sigma*sqrt(h)); // up ratio
	double d=exp(-sigma*sqrt(h));// down ratio
	double p=(exp((r)*h)-d)/(u-d); // risk neutral probability
	double discount=exp(-r*h);

	double current[301];
	double stockT[301];

	//Initialization for tree at time T
	for (int i=0;i<N+1;i++)
	{
		stockT[i]=S*pow(u,N-i)*pow(d,i);
		if (iscall!=0)
		{	//Call Option
			if (stockT[i]>K) 
				current[i]=stockT[i]-K;
			else
				current[i]=0;
		}
		else
		{	//Put Option
			if (stockT[i]<K) 
				current[i]=K-stockT[i];
			else
				current[i]=0;
		}
	}

	//Start Recursive Calculation  
	double temp,recur;
	for (int i=N;i>0;i--)
	{
		for (int j=0;j<i;j++)
		{
			//American option
			recur=discount*(p*current[j]+(1-p)*current[j+1]);

			temp=S*pow(u,i-1-j)*pow(d,j);//current price
			if (iscall!=0)
			{	//Call Option
				if (temp>K) 
					temp=temp-K;
				else
					temp=0;
			}
			else
			{	//Put Option
				if (temp<K) 
					temp=K-temp;
				else
					temp=0;
			}

			//Early excercise or not
			if (temp>recur)
				current[j]=temp;
			else
				current[j]=recur;
		}
	}
	return current[0];
}


