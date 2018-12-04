#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	double start, end;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	bool active = true; // false = pasive
	bool ring_leader = false;
	int ring_leader_rank;
	int arr[5][2] = {
		{1,2},
		{3,0},
		{0,4},
		{4,1},
		{3,2}
	};

	printf("Rank %d left: %d and right: %d\n", rank, arr[rank][0], arr[rank][1]);

	/* while (!ring_leader)
	{
		if (active)
		{
			
			// send id to left
			// send id to right
		}
		else
		{
			// if msg recieved from a neighbor just pass that id
		}
	} */

	if (rank == 0)
	{
		//printf("Leader is %d\n",ring_leader_rank);
	}

	MPI_Finalize();
	return 0;
}