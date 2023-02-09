// Parametros
int nJobs = ...;
int nMchs = ...;

range Jobs = 1..nJobs;
range Mchs = 1..nMchs;

int P[Jobs][Mchs] = ...;
int DeliveryDate[Jobs] = ...;

// Variaveis de decisao
dvar int+ X[Jobs][Jobs];
dvar int+ C[Jobs];
dvar int+ E[Jobs];
dvar int+ T[Jobs];

dvar int+ W[Jobs][1..nMchs-1];
dvar int+ I[1..nJobs-1][Mchs];
dvar int+ I0;

// funcao objetivo
minimize
	sum (j in Jobs) (E[j] + T[j]);

subject to {
  
  	C[1] == I0 + sum(k in Mchs) sum(i in Jobs) X[i][1]*P[i][k] + sum(i in 1..(nMchs-1)) W[1][i];
  	
  	forall (j in 2..nJobs)
  	  	C[j] == C[j-1] + I[j-1][nMchs] + sum(i in Jobs) X[i][j]*P[i][nMchs];
  	  	
  	forall(j in 1..(nJobs-1))
  	  	forall(k in 1..(nMchs-1))
  	  	  	I[j][k] + (sum(i in Jobs) X[i][j+1]*P[i][k]) + W[j+1][k] == W[j][k] + (sum(i in Jobs) X[i][j] * P[i][k+1]) + I[j][k+1];
  
	forall (i in Jobs){
	  	sum (j in Jobs) X[i][j] == 1;
	
	  	sum (j in Jobs) X[j][i] == 1;
		
		
		T[i] >= C[i] - sum(j in Jobs) X[j][i]*DeliveryDate[j];
		E[i] >=  sum(j in Jobs) X[j][i]*DeliveryDate[j] - C[i];
	}
	

	
}

execute {
  	write("Ordem tarefas: ( ")
  	for(var i = 1; i <= nJobs; i++){
  	  for(var j = 1; j <= nJobs; j++){
  	    if(X[j][i] == 1) write(j + " ");
  	  }
  	}
  	write(")");
}
