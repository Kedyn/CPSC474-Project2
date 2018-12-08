#include "mpi.h"
#include <stdio.h>

int NEIGHBORS[5][2] = {
	{1,2},
	{3,0},
	{0,4},
	{4,1},
	{3,2}
};

int ACTIVES[5] = {
	1,
	0,
	1,
	0,
	0
};

int main(int argc, char *argv[])
{
	int rank, size;
	double start, end;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	MPI_Status status;

	int round_turned_passive = 0;

	int send_msg[3] = { rank, 1, -1 }; // passing id, round, leader
	int recv_msg[3];

	if (ACTIVES[rank])
	{
		MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
		MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor
	}

	while (send_msg[2] == -1)
	{
		MPI_Recv(&recv_msg, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		printf("Rank: %d Receive msg from %d: (%d, %d, %d).\n", rank, status.MPI_SOURCE, recv_msg[0], recv_msg[1], recv_msg[2]);

		if (ACTIVES[rank])
		{
			if (recv_msg[0] > rank)
			{
				ACTIVES[rank] = 0;

				round_turned_passive = recv_msg[1];
			}
			else if (recv_msg[0] == rank)
			{
				send_msg[2] = rank;

				printf("Rank: %d is now leader.\n", send_msg[2]);
			}

			if (ACTIVES[rank])
			{
				send_msg[1]++;

				MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
				MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor
			}
		}
		else
		{
			if (recv_msg[1] != round_turned_passive)
			{

				if (status.MPI_SOURCE == NEIGHBORS[rank][0])
				{
					if (recv_msg[2] != NEIGHBORS[rank][1])
					{
						printf("Rank: %d Passing msg to: %d.\n", rank, NEIGHBORS[rank][1]);
						MPI_Send(&recv_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor
					}
				}
				else
				{
					if (recv_msg[2] != NEIGHBORS[rank][0])
					{
						printf("Rank: %d Passing msg to: %d.\n", rank, NEIGHBORS[rank][0]);
						MPI_Send(&recv_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
					}
				}

				if (recv_msg[2] != -1)
				{
					send_msg[2] = recv_msg[2];

					printf("Rank: %d has set %d as leader.\n", rank, send_msg[2]);
				}
			}
		}
	}

	MPI_Finalize();

	if (rank == 0)
	{
		printf("Leader: %d.\n", send_msg[2]);
	}

	return 0;
}