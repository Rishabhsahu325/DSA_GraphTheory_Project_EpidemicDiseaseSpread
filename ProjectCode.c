#include<stdio.h>
#include<assert.h>
#include<math.h>//pow and sqrt function//IF COMPILING ON TERMINAL USE "-lm" beside the compiling gcc command because this library has been used
#include<stdlib.h>//rand() function

//list of all functions GIVEN IN PDF
int randomYes();// BY RISHABH 
//used to create service providers, infected persons and indirectly in biased yes

int gcd(int a,int b);//find gcd of a and b ,used in countPRimeFactors function 
int countPrimeFactors(int a,int b);// BY NITISH

int biasedYes();// returns 1 more frequently than biased yes ,required to set up adjacency matrix


//List of other functions or parts of do_experiment and main function not in pdf
void DFS(int i,int p,int ADJ[p][p],int visited[p]);//or use pointer to array like int * whichever suits better , //BY SATISH AND PAWAN
int calculate_no_to_be_tested(int p,int matrix[p][p]);//count number of 1's in this 2d array // BY PREMANSH ,more description on line no 44
int doExperiment(int p,int s,int ir,char t);

double calculate_standard_deviation(int arr[10]);// BY ARPAN
//calculate the standard deviation of the 10 integers in the array

//function definitions
int randomYes()
{
	return ((rand()%1000)<1);
}

int gcd(int a,int b)//find gcd of a and b
{
	if(a==0)
		return b;
	if(b==0)
		return a;
	if(a==b)
		return a;
	if(a>b)
		return gcd(a-b,b);
	return gcd(a,b-a);
}
// Count of common prime factors 
int countPrimeFactors(int a, int b) //count number of prime factors in the prime factorisation of the gcd of a and b
{ 
    
    int x = gcd(a, b); 
  	
	int res = 0; 
    if (x % 2 == 0) { 
    	res++; 
  
        
        while (x % 2 == 0) 
        	x = x / 2; 
    } 
  
   
    for (int i = 3; i*i <= (x); i = i + 2) { 
        if (x % i == 0) { 
        	res++; 
        	while (x % i == 0) 
            	x = x / i; 
        } 
    } 
  
    
    if (x > 2) 
        res++; 
    return res; 
}
int biasedYes(int x, int y)
{ 
/* Where x and y and indices of persons in Adjacency matrix. */
	int common = countPrimeFactors(x+1001, y+1001); /* Define function countPrimeFactors() that computes the Count of prime factors common in two arguments.
More prime factor they have in common, more often the pair meets!
*/
// Return biased interaction 
	while(common--)
	if (randomYes() == 1)
		return 1;
	return 0;
}
void DFS(int i,int p,int ADJ[p][p],int visited[p])
{
	int j;//number used to traverse through the i th row in adjacency matrix
	visited[i]=1;//change the value in visited array so that we don't visit this matrix again (for the members not initially present in the infected matrix)

	for(j=0;j<p;j++)
	{
	    if(ADJ[i][j]==1 && visited[j]==0)
		{
			DFS(j,p,ADJ,visited);
			ADJ[i][j]=-1;
		} 
	}
    
}


//these function names are long to describe their work, but we will change them to shorter names

int calculate_no_to_be_tested(int p,int matrix[p][p])//take 2d array as parameter for this function from outside the function,if you find any possible way then keep the size of that 2d array variable , else use an array with size 10000
{
	//take some 2d integer array as input parameter within the round brackets or use the matrix below if you are unable to accept 2d array with variable size as input
	
	int t;//store the answer here
	int i,j;
	for(i=0;i<p;i++)
	{
	    for(j=0;j<p;j++)
	    {
	        if(matrix[i][j]==-1)
	       		t++;
	    }
	}
	return t;
}


int doExperiment(int p,int s,int ir,char t)
{

	/*use these assert  conditions after testing is finished
	assert(p==2000||p==5000||t==10000);
	assert(s==3||s==8);
	assert(ir==5||ir==30||ir==100);
	assert(t==w||t==x||t==y||t==z);
	*/
//Prearrangements :declare an adjacency matrix and check if it gets properly created before writing further code in this function 
	int ADJ[p][p];
//just checking that array values are properly initialised to 0 or do we have to initialise them ourselves
	for(int a=0;a<p;a++)
	{
		for(int b=0;b<p;b++)
		{
			assert(ADJ[a][b]==0);
		}

	}	
	//IMPORTANT! IMPORTANT! IMPORTANT!ok now you know this is important so i tell you, i have compared Exact values OF serv_interact_count and norm_count to set up person to person as well as service provider interactions but in the original document it is specified that 10 % range is permissible
	//SO IF THE PROGRAM TAKES UP TOO MUCH TIME TO RUN OR INFINITE TIME FOR THE LARGE  P VALUES WE NEED TO MODIFY THE CONDITIONS AND ACCOMODATE THE +=10% RANGE INSTEAD OF THE EXACT VALUES TO REDUCE RUNTIME
// Task 1 : set up an adjacency matrix
	for(int a=0;a<p;a++)
	{
		for(int b=0;b<p;b++)
		{
			ADJ[a][b]=0;
		}

	}

	//subtask a: setup service providers using randomyes()
	int countS=(p*s)/100;//count of number of service providers that need to be created
	int serviceProv[p];
	for(int k=0;k<p;k++)
		serviceProv[k]=0;
	int c=0;
	while(c!=countS)
	{
		for(int k=0;k<p;k++)
		{
			if((randomYes()==1)&&(serviceProv[k]==0))//set up person as service provider if randomYes returns 1 and it is not a service provider already
			{
				serviceProv[k]=1;
				c++;
			}
			if(c==countS)
				break;
		}
	}
	printf("Service providers initialised\n");
	printf("countS= %d\n",countS);

//print service provider array to verify
	printf("printing service provider array for checking,remove this segment of code after program is tested correctly \n")
	for(int k=0;k<p;k++)
		printf("%d\t",serviceProv[k]);
	printf("\n");
	
	//subtask b :check the social distancing parameter and set up values of serv_interact_count as well as person to person count according to that which are used as it is in subtask c 
	
	//for t100 parameter we setup the values in the default case of switch statement

	int serv_interact_count;//total number of service provider to person interaction in whole population
	switch(s)
	{
		// case first :very few providers i.e. number of service providers is 3% of the town population;
		//Each service provider comes in contact with about 5% of the town population if the town has low number of providers.
		case 3: serv_interact_count=(5*p*countS)/100;break;
		//case second :if there are numerous service-providers then number of service providers in the town is 8% of the population.
		//In town with many service providers, each provider only interacts with 3% of the town population in a week
		case 8: serv_interact_count=(3*p*countS)/100;break;
		default : 
		{	
			printf("service level does not match the options in the document ");
			exit(0);
		}
	}
	//20 interactions/person is AVERAGE for person-to-person.
	int norm_count=20*p;//store the number of normal person to normal person interactions needed
	int tc=0;
	switch(t)
	{
		case 'w':norm_count=20*p;break;//t100 parameter
		case 'x':norm_count/=2;break;//t50 parameter
		case 'y':norm_count/=3;break;//t33 parameter
		case 'z':norm_count/=2;tc=1;break;// int tc =1 will imply the tc parameter, and then only the odd numbered person will interact with odd numbered person and even with even
	}
	//subtask c: setup entries in the adjacency matrix using randomyes() and biasedyes(), total number of entries ofnormal person to normal person interaction need to be  equal to 20*p*(1-s) + or - 10%

	//Use biasYes() set up interactions of both kind. i.e. service provider to people, and people to people
		//service provider interaction

			//when tc=0	
	if(tc==0)
	{
		while(serv_interact_count!=0)
		{
			for(int a=0;a<p;a++)//traverse through the array containing service providers
			{	
				if(serviceProv[a]==1)//pick up service provider
				{
					for(int b=0;b<p;b++)//traverse through all other people other than the service provider
					{
						if((biasedYes(a,b)==1)&&(ADJ[a][b]==0)&&(b!=a))//if biased yes returns 1, interacton was initially 0 , and the a th service provider is not the b th person
						{
						
							ADJ[a][b]=1;//setup this new interaction for the service provider
							ADJ[b][a]=1;//interaction is bidirectional
							serv_interact_count--;
						}
						if(serv_interact_count==0)
							break;
							
					}
				}	
				if(serv_interact_count==0)
					break;
			}
		}
	}
	//with the tc=1 parameter
	else if(tc==1)
	{
		while(serv_interact_count!=0)
		{
			for(int a=0;a<p;a++)//traverse through the array containing service providers
			{	
				if(serviceProv[a]==1)//pick up service provider
				{
					if(a%2==0)//checking whether service provider is odd numberered or even numbered
					{
						for(int b=0;b<p;b+=2)//traverse through all even numbered people other than the service provider
						{
							if((biasedYes(a,b)==1)&&(ADJ[a][b]==0)&&(b!=a))//if biased yes returns 1, interacton was initially 0 , and the a th service provider is not the b th person
							{
								
								ADJ[a][b]=1;//setup this new interaction for the service provider
								ADJ[b][a]=1;//interaction is bidirectional
								serv_interact_count--;
							}
							if(serv_interact_count==0)
								break;
								
						}
					}
					else//with odd numbered service provider
					{
						for(int b=1;b<p;b+=2)//traverse through all odd numbered people other than the service provider
						{
							if((biasedYes(a,b)==1)&&(ADJ[a][b]==0)&&(b!=a))//if biased yes returns 1, interacton was initially 0 , and the a th service provider is not the b th person
							{
								
								ADJ[a][b]=1;//setup this new interaction for the service provider
								ADJ[b][a]=1;//interaction is bidirectional
								serv_interact_count--;
							}
							if(serv_interact_count==0)
								break;
								
						}
					}
				}		
				if(serv_interact_count==0)//condition to stop for loop containing a , when enough interactions are set up
					break;
				
			}//end of for (int a...
		}//end of while
	}//end of else if tc==1
	
	//set up normal person to normal person interactions
	while(norm_count!=0)
	{
		for(int m=0;m<p;m++)//traverse through list of people
		{	
			
			for(int n=0;n<p;n++)//traverse through all other people other than the service provider
			{
				if((biasedYes(m,n)==1)&&(ADJ[m][n]==0)&&(n!=m))//if biased yes returns 1, interacton was initially 0 , and the m th person is not the n th person then set up the interaction
				{
						ADJ[m][n]=1;//setup this new interaction for the service provider
						ADJ[n][m]=1;//interaction is bidirectional
						norm_count--;
				}
					if(norm_count==0)
						break;
			}	
		}
		if(norm_count==0)
			break;
	}//end of while loop used for normal interactions
		
		
		
	//subtask c :make some people infected with the virus using randomyes() ,by listing them in a linear array
	//initialise infected matrix
	int infected[p];
	for(int i=0;i<p;i++)
    		infected[i]=0;
	while(ir!=0)
	{
		for(int k=0;k<p;k++)
		{
			if((randomYes()==1)&&(infected[k]==0))//set up person as service provider if randomYes returns 1 and it is not a service provider already
			{
				infected[k]=1;
				ir--;
			}
			if(ir==0)
				break;
		}
	}
	
//task 2 return number of people to be tested
	//SUBTASK ALPHA : edit the entries in adjacency matrix to list people to be tested
	for(int i=0;i<p;i++)
	{
		if(infected[i]==1)
			DFS(i,p,ADJ,infected);
	}
	//SUBTASK BETA : calculate the number of people to be tested
	
	return calculate_no_to_be_tested(p,ADJ);

}//end of doExperiment

double calculate_standard_deviation(int arr[10])
{
	double sum = 0.0, mean, sd = 0.0;
	int a;
	for (a = 0; a < 10; ++a)
	{
    	sum += arr[a];
    }
	mean = sum / 10;
    for (a = 0; a < 10; ++a)
	{
    	sd += pow(arr[a] - mean, 2);
	}
    
    return sqrt(sd / 10);
}



int main(void)
{

	int repeats;
	int p,s,ir;
	char t;
	int results[10];
	int sum ;
	double avg;
	printf("select social distancing parameter among the following options");
	printf("description for each parameter is this \n Without any social distancing means(T100)\n With social distancing reducing contacts to 50%% means(T50)\n With social distancing reducing contacts to 33%% means(T33)\n With social distancing reducing contacts to 50%% with assigned service provider means(TC)\n\n");
	//here %% will print a single percentage symbol
	printf("choose w for t100 parameter\n");
	printf("choose x for t50 parameter\n");
	printf("chosse y for t33 parameter\n");
	printf("choose z for tc parameter\n\n");
	scanf("%c",&t);
	
	printf("Enter the values of \nPopulation size(2000,5000,10000);\nService level (3or 8);\nNumber of infected persons(5,30,100)\nrespectively ,each separated by a single space\n");
	// accept input values p, s,ir ,t (social distancing parameter)
	scanf("%d %d %d",&p,&s,&ir);
	
	
	
	for (repeats = 0; repeats < 10; repeats++)
		results[repeats] = doExperiment(p,s,ir,t);
	
	for( int q=0;q<10;q++)
		sum=sum+results[q];
	avg=sum/10;
// Compute average and std dev. 
	printf("Average of 10 experiments is %lf\n",avg);
	printf("Standard deviation of 10 experiments is %lf\n",calculate_standard_deviation(results));

}


//not useful after testing is completed
/*
//void main for testing, renamed visited matrix as infected to make the purpose clear
void main()
{
	int p=5;
	int ADJ[p][p],infected[p],V=p;
//just checking that array values are properly initialised to 0 or do we have to initialise them ourselves
	for(int i=0;i<p;i++)
 	   	infected[i]=0;


//test case initialization
	for(int a=0;a<p;a++)
	{
		for(int b=0;b<p;b++)
		{
			if((a==0)^(b==0))//either a or b is 0 but not both
				ADJ[a][b]=1;
			else
				ADJ[a][b]=0;
		}

	}
	infected[1]=1;
	infected[2]=1;
//printing matrix before calling dfs function
	for(int a=0;a<p;a++)
	{
		for(int b=0;b<p;b++)
		{
			printf("%d\t",ADJ[a][b]);
		}
		printf("\n");

	}
	printf("\n");

	//int countS=(p*s)/100;//count of number 0f service providers that need to be created

//infected array will store infected persons i.e if infected[i]==1 that means ith person is initially infected.
	for(int i=0;i<p;i++)
	{
		if(infected[i]==1)
			DFS(i,p,ADJ,infected);
	}
	
	//printing matrix to check final output
	for(int a=0;a<p;a++)
	{
		for(int b=0;b<p;b++)
		{
			printf("%d\t",ADJ[a][b]);
		}
		printf("\n");
	}
}

*/
