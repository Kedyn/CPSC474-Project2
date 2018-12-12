#include "mpi.h"
#include <stdio.h>

/* int NEIGHBORS[5][2] = {
	{1, 2},
	{3, 0},
	{0, 4},
	{4, 1},
	{3, 2}
}; */
int NEIGHBORS[14][2] = {
	{10, 13},
	{6, 10},
	{7, 11},
	{9, 6},
	{11, 12},
	{13, 8},
	{3, 1},
	{8, 2},
	{5, 7},
	{12, 3},
	{1, 0},
	{2, 4},
	{4, 9},
	{0, 5}
};

/* int ACTIVES[5] = {
	0,
	1,
	1,
	0,
	1
}; */
int ACTIVES[14] = {
	0,
	0,
	0,
	1,
	1,
	1,
	0,
	0,
	0,
	1,
	0,
	1,
	0,
	0,
};

int main(int argc, char *argv[])
{
	int rank, size;
	double start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;

	int round_turned_passive = 0;
	int round_msgs = 0;

	int send_msg[3] = { rank, 1, -1 }; // passing id, round, leader
	int recv_msg[3];

	char *state = "passive";

	if (ACTIVES[rank])
	{
		state = "active";

		printf("Rank: %d Sending msg to %d and %d.\n", rank, NEIGHBORS[rank][0], NEIGHBORS[rank][1]);

		MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
		MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor
	}

	while (send_msg[2] == -1)
	{
		MPI_Recv(&recv_msg, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		printf("Rank: %d Receive msg from %d: (%d, %d, %d).\n", rank, status.MPI_SOURCE, recv_msg[0], recv_msg[1], recv_msg[2]);

		if (ACTIVES[rank])
		{
			round_msgs++;

			if (recv_msg[0] > rank)
			{
				printf("Rank: %d is now passive because of %d.\n", rank, recv_msg[0]);

				ACTIVES[rank] = 0;

				round_turned_passive = recv_msg[1];

				state = "passive";

				round_msgs = 0;
			}
			else if (recv_msg[0] == rank && round_msgs == 2)
			{
				send_msg[2] = rank;

				printf("Rank: %d is now leader.\n", rank);
			}

			if (ACTIVES[rank] && round_msgs == 2 && recv_msg[2] != rank)
			{
				send_msg[1]++; // increment round

				printf("Rank: %d Sending msg to %d and %d.\n", rank, NEIGHBORS[rank][0], NEIGHBORS[rank][1]);

				MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
				MPI_Send(&send_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor

				round_msgs = 0;

				send_msg[2] = recv_msg[2];
			}
		}
		else
		{
			if (recv_msg[1] != round_turned_passive)
			{
				if (status.MPI_SOURCE == NEIGHBORS[rank][0])
				{
					//if (recv_msg[2] != NEIGHBORS[rank][1])
					//{
						printf("Rank: %d Passing msg to: %d.\n", rank, NEIGHBORS[rank][1]);

						MPI_Send(&recv_msg, 3, MPI_INT, NEIGHBORS[rank][1], 1, MPI_COMM_WORLD); // send to right neighbor
					//}
				}
				else
				{
					//if (recv_msg[2] != NEIGHBORS[rank][0])
					//{
						printf("Rank: %d Passing msg to: %d.\n", rank, NEIGHBORS[rank][0]);

						MPI_Send(&recv_msg, 3, MPI_INT, NEIGHBORS[rank][0], 1, MPI_COMM_WORLD); // send to left neighbor
					//}
				}

				if (recv_msg[2] != -1)
				{
					round_msgs++;

					if (round_msgs == 2)
					{
						send_msg[2] = recv_msg[2];

						printf("Rank: %d has set %d as leader.\n", rank, send_msg[2]);
					}
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