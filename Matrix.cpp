//Matrix.cpp
//function of class matrix

#include "Matrix.h"

matrix::matrix(string name)
{
	ifstream fin(name.c_str());
	while (fin.peek() == '%')fin.ignore(2048, '\n');
	fin >> m >> n >> hnz;
	count1 = 0;
	int *row = new int[hnz];
	int *colomn = new int[hnz];
	double *value = new double[hnz];
	for (int i = 0; i < hnz; i++)            //read input
	{
		fin >> row[i] >> colomn[i] >> value[i];
		if (row[i] == colomn[i])
			count1++;
	}
//	nz = 2 * hnz - count1;
	I = new int[hnz];
	J = new int[hnz];
	VAL = new double[hnz];
//	int k = hnz;
	for (int i = 0; i < hnz; i++)
	{
		I[i] = row[i] - 1;             //change the index, start element of a matrix market format is (1,1), not (0,0) 
		J[i] = colomn[i] - 1;
/*		if (I[i] != J[i])         //fill the non-zero element on upper half of the symmetric matrix
		{
			I[k] = J[i];
			J[k] = I[i];
			VAL[k] = value[i];
			k++;
		}*/
		VAL[i] = value[i];
//		cout<<I[i]<<" "<<J[i]<<" "<<VAL[i]<<endl;
	}                    // COO format complete  I J VAL
	for (int i = 0; i < hnz; i++)
		cout<<I[i]<<" ";
	cout<<endl;
	for (int i = 0; i < hnz; i++)
		cout<<J[i]<<" ";
	cout<<endl;
	for (int i = 0; i < hnz; i++)
		cout<<VAL[i]<<" ";
	cout<<endl;
	delete[]row;
	delete[]colomn;
	delete[]value;
}

void matrix::converthalfCSC()
{
	int *clp=new int[m];
	for(int i=0;i<m;i++)clp[i]=0;
	for(int i=0;i<m;i++){
	for (int j=0;j<hnz;j++)
	{
		if(J[j]==i)clp[i]++;
	}
	}	
	col_ptr=new int[m+1];
	col_ptr[0]=0;
	int temp=0;
//	for (int j=0;j<hnz;j++)cout<<VAL[j]<<" "<<I[j]<<endl;
//	cout<<col_ptr[0]<<" ";
	for (int i = 1; i < m + 1; i++)
	{
	//	cout << counter[i] << endl;
		col_ptr[i] = temp + clp[i - 1];
//		cout<<col_ptr[i]<<" ";
		temp = col_ptr[i];
	}
	cout<<endl;
	delete[]clp;
	for (int i = 0; i < hnz; i++)
		cout<<VAL[i]<<" ";
	cout<<endl;
	for (int i = 0; i < hnz; i++)
		cout<<I[i]<<" ";
	cout<<endl;
	for (int i = 0; i < m+1; i++)
		cout<<col_ptr[i]<<" ";
	cout<<endl;

}      //half CSC COMPLETE:  VAL,I,col_ptr




void matrix::convertfullCSC()
{
	IA = new int[nz];
	JA = new int[nz];
	val = new double[nz];
	int newi = 0;
	for (int k = 0; k<m; k++)            //the input contains data in one colomn by one 
	{
		for (int i = 0; i < nz; i++)
		{
			if (I[i] == k)
			{
				IA[newi] = I[i];
				JA[newi] = J[i];
				val[newi] = VAL[i];
	//			cout << " " << JA[newi] << " " << val[newi] << endl;
				newi++;
			}
		}
	}
	cout << endl;
	ia = new int[m + 1];
	counter = new int[m];
	for (int i = 0; i < m; i++)counter[i] = 0;
	ia[0] = 0;
	int temp = 0;
	for (int k = 0; k < nz; k++)
		counter[IA[k]]++;

	for (int i = 1; i < m + 1; i++)
	{
	//	cout << counter[i] << endl;
		ia[i] = temp + counter[i - 1];
	//	cout<<ia[i]<<endl;
		temp = ia[i];
	}

}

void matrix::generateetree()
{
	struct Etree* etree=createEtree(m);
	
	for(int j=1;j<m;j++){
	for(int i=0;i<j;i++)
	{
		for(int k=col_ptr[i];k<col_ptr[i+1];k++)
		{	
			if(I[k]==j)
				addEdge(etree,i,j);
		}
	}
}

	printEtree(etree);
	checkfillin(etree);
//	output(xx);
//	output(yy);
}

void matrix::lfactorstructure()
{
	vector <int>::const_iterator c1;
	vector <int>::const_iterator c2;
	for(int i=0;i<hnz;i++)
		for(c1=xx.begin(),c2=yy.begin();c1!=xx.end()&&c2!=yy.end();++c1,++c2)
		{	if(*c1==I[i]&&*c2==J[i])
			{
				cout<<*c1<<" "<<*c2<<endl;
				xx.erase(c1);
				yy.erase(c2);
			}	
		}
	cout<<"After filter, final fill in, "<<xx.size()<<" "<<yy.size()<<endl;
	for(c1=xx.begin(),c2=yy.begin();c1!=xx.end()&&c2!=yy.end();++c1,++c2)
		cout<<"[x y] "<< *c1<< " "<<*c2<<endl;
}

void matrix::cholesky()
{
	vector <int> newi(I,I+hnz);
	vector <int> newcol_ptr(col_ptr,col_ptr+m+1);
	vector <double> newval(VAL,VAL+hnz);
	output(newi);
	output(newcol_ptr);
	output(newval);
	vector <int>::const_iterator c1;
	vector <int>::const_iterator c2;
	cout<<"insert fill in"<<endl;
//for(int k=0;k<xx.size();k++)
	for(c1=xx.begin(),c2=yy.begin();c1!=xx.end()&&c2!=yy.end();++c1,++c2)
	{
		for(int i=0;i<m;i++)
		{
			if(*c2==i)
			{
			
			for(int k=newcol_ptr[i];k<newcol_ptr[i+1];k++)
				{	
					if((*c1>newi[k]&&*c1<newi[k+1]&&k+1<newcol_ptr[i+1])
						||(*c1>newi[k]&&k+1==newcol_ptr[i+1]&&*c1>=newi[k+1]))
					{
						newi.insert(newi.begin()+k+1,*c1);
						newval.insert(newval.begin()+k+1,0);
					}
				}
			for(int j=i+1;j<m+1;j++)
					newcol_ptr[j]++;
			}
		}
	}	
	cout<<"After process, the input is "<<endl;
	output(newi);
	output(newcol_ptr);
	output(newval);
	clock_t begin = clock();
	for(int i=0;i<m;i++)
	{
//		cout<<i<<"th column "<<endl;
		newval[newcol_ptr[i]]=sqrt(newval[newcol_ptr[i]]);
		for(int k=newcol_ptr[i]+1;k<newcol_ptr[i+1];k++)
			newval[k]/=newval[newcol_ptr[i]];
		if(i<m-1)
		{
			vector <double> subnewval;
			vector<int> subrow;
			vector <int> subcol;
			for(int q=newcol_ptr[i]+1;q<newcol_ptr[i+1];q++)
			{
				for(int j=q;j<newcol_ptr[i+1]&&j>=q;j++)
					{
						subnewval.push_back(newval[j]*newval[q]);
						subrow.push_back(newi[j]);
						subcol.push_back(newi[q]);
					}
			}
	//		output(subrow);
	//		output(subcol);
	//		output(subnewval);
	//		cout<<subcol.size()<<endl;
			for(int p=i+1;p<m;p++)
				for(int j=newcol_ptr[p];j<newcol_ptr[p+1];j++)
					for(int l=0;l<subcol.size();l++)
						if(subcol[l]==p&&newi[j]==subrow[l])
							newval[j]-=subnewval[l];
		}
/*		cout<<"now the matrix :"<<endl;
		output(newi);
		output(newcol_ptr);
		output(newval);
		*/
	}
	printf("Run time: %.6fs\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
	cout<<"result"<<endl;
	output(newi);
	output(newcol_ptr);
	output(newval);
}

template <typename T> void matrix::output(const vector <T> &array)
{
	typename vector <T>::const_iterator constIterator;
	for(constIterator=array.begin();constIterator!=array.end();++constIterator)
		cout<<*constIterator<<" ";
	cout<<endl;
}

matrix::~matrix()
{
	delete[]I;
//	delete[]IA;
	delete[]J;
//	delete[]JA;
//	delete[]val;
	delete[]VAL;
//	delete[]ia;
//	delete[]counter;
//	delete[]row_ind;
	delete[]col_ptr;
}
