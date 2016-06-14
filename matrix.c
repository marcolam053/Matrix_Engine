#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <immintrin.h>
#include "pagerank.h"
#define max_thread 5

// Create M matrix
void init_matrix(node* list, size_t npages, double *matrix,int g_elements,int g_height, int g_width){
	node *temp = list;
	node *in = list;

	for(int row = 0 ; row < g_width; row++){
		for(int col = 0; col < g_height; col++){
			// for no outlinks, 1/npages
			if(temp->page->noutlinks == 0){
				matrix[row*g_height+col] = 1/npages;
			}else{
				// Iterate throught the row of the martrix M
				while(in->page->index  != row){
					in = in->next;
				}
				in = in->page->inlinks;
				// have in
				while(in->page->inlinks!=NULL){
					// Check if the col index has a path to row  index(vice versa)
					if(in->page->index == col) break;
					in = in->next; // if no out(j) to next
				}
				in = list;
				while(in != NULL){
					if(in->page->index == col){
						while(in != NULL){
							if(in->page->index == col){
								matrix[row*g_height+col] = 1/(in->page->noutlinks);
								break;
							}
							// find nexr node with outlinks
							in = in -> next;
						}
					}
					// if no out links -> next node
					in = in -> next;
				}
			}
			in = in->next;
		}
	}
}

// Initiation of m head matrix
void init_m_hat(node* list, double dampener,size_t npages,double *hat_matrix, double *matrix,int g_elements, int g_height, int g_width){

	// product between num and E matrix;
	for(int j = 0; j < g_elements;j++){
		hat_matrix[j] = 1.00000000*((1-dampener)/npages);
	}
	// Product between dampener and the matrix M
	for(int k = 0; k < g_elements; k++){
		matrix[k] = matrix[k]*dampener;
	}
	// Sum the two product above
	for(int h = 0; h < g_elements;h++){
		hat_matrix[h] = hat_matrix[h]+matrix[h];
	}
}

double* ranking(size_t nedges, size_t npages, double dampener,double *hat_matrix,int g_elements, int g_width, int g_height){

				// Store the result of the ranking
				double *result = calloc(g_elements,sizeof(double));
				// Store the p(n)matrix
				double *p_matrix = calloc(g_elements,sizeof(double));
				// Store the p(n-1) matrix
				double *p_n_matrix = calloc(g_elements,sizeof(double));

				double *temp = calloc(g_elements,sizeof(double));
				int iteration = 0;

			for(int i = 1; 0<1; i++){

				if(iteration == 0){
					for(int a = 0; a < g_elements;a++){
							p_n_matrix[a]=1/npages
						}
				}

				if(iteration != 0){
					for(int j = 0; j < g_elements; j++){
						p_n_matrix[j] = p_matrix[j];
					}
				}

				// Calculate p(n) (new)
					for(int h = 0; h < g_width; h++){
						for(int k = 0; k < g_height; k++){
							for(int g = 0; g < g_width; g++){
								p_matrix[h*g_height+k] = hat_matrix[h*g_height+k] + hat_matrix[h*g_height+g]*p_n_matrix[g*g_height+k];
							}
						}
					}

				// calculate p(n)-p(n-1)
				for(int x = 0; x <g_elements;x++){
					temp[x] = p_matrix[x] - p_n_matrix[x];
				}

				// Calculate the square sum of temp
				double sum = 0;
				for(int t = 0; t < g_elements; t++){
					sum += temp[t]*temp[t];
				}

				// square root of the sum
				if(sqrt(sum) < EPSILON){
					break;
				}else{
					iteration++;
					continue;
				}
			}
			// put matrix p into the result matrix after finished
			for(int i = 0; i < g_elements; index++,i++){
				result[i] = p_matrix[i];
			}

			free(p_matrix);
			free(p_n_matrix);
			free(temp);
			
			return result;
}


void pagerank(node* list, size_t npages, size_t nedges, size_t nthreads, double dampener) {

	/*
	- implement this function
	- implement any other necessary functions
	- implement any other useful data structures
	*/
	int g_elements = npages*npages;
	int g_height = npages;
	int g_width = npages;


	double *matrix = calloc(g_elements, sizeof(double));
	double *hat_matrix = malloc(g_elements*sizeof(double));

	// Initiate the page rank matrix :)
	init_matrix( list,npages, matrix,g_elements,g_height,g_width);
	init_m_hat(list,dampener,npages,hat_matrix,matrix,g_elements,g_height,g_width);
	// Calculate the Page Rank and print :)
	ranking(nedges,npages,dampener,hat_matrix,g_height,g_elements,g_width);


	free(matrix);
	free(hat_matrix);
}

/*
######################################
### DO NOT MODIFY BELOW THIS POINT ###
######################################
*/

int main(int argc, char** argv) {

	/*
	######################################################
	### DO NOT MODIFY THE MAIN FUNCTION OR HEADER FILE ###
	######################################################
	*/

	config conf;

	init(&conf, argc, argv);

	node* list = conf.list;
	size_t npages = conf.npages;
	size_t nedges = conf.nedges;
	size_t nthreads = conf.nthreads;
	double dampener = conf.dampener;

	pagerank(list, npages, nedges, nthreads, dampener);

	release(list);

	return 0;
}

