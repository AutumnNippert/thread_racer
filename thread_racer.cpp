#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>

#include <cmath>
#include <cstdlib>
#include <random>

#include <latch>
#include <stop_token>

#define HORSE_EMOJI ">"
#define PROGRESS_BAR_CHAR "▮"

void racer(int id, size_t num_iterations);
void run_race(size_t num_threads, size_t num_iterations);

std::vector<std::jthread> threads;
std::vector<size_t> racer_progress;

void print_n_times(std::string str, size_t n){
	for(size_t i = 0; i < n; i++){
		std::cout << str;
	}
}

void print_progress_bar(size_t progress, size_t total){
	size_t progress_bar_length = 100 * progress / total;

	std::cout << "[";
	for (size_t j = 0; j < progress_bar_length; j++)
	{
		std::cout << PROGRESS_BAR_CHAR;
	}
	std::cout << HORSE_EMOJI;
	print_n_times("-", 100 - progress_bar_length);
	std::cout << "]";
}

int main(int argc, char const *argv[])
{
	// get the number of threads from the command line
	size_t num_threads = 1;
	if (argc > 1)
	{
		num_threads = std::stoi(argv[1]);
	}

	// get the number of iterations from the command line
	size_t num_iterations = 10000;
	if (argc > 2)
	{
		num_iterations = std::stoi(argv[2]);
	}

	// initialize the racer progress
	racer_progress = std::vector<size_t>(num_threads, 0);

	// run the race
	run_race(num_threads, num_iterations);
	return 0;
}

void busyWait(size_t calculations) {
    double x = 0.0;
	for (size_t i = 0; i < calculations; ++i) {
		x = sin(static_cast<double>(rand()) / RAND_MAX + x);
	}
}

void run_race(size_t num_threads, size_t num_iterations)
{
	std::stop_source ss;
	std::latch start_latch = std::latch(num_threads);
	std::atomic<bool> stop = false;
	
	auto racer = [&start_latch, &stop, &ss](int id, std::stop_token st, size_t num_iterations, double multiplier)
	{
		// wait for all threads to start
		start_latch.arrive_and_wait(); // decreases the latch by 1 and waits for the latch to reach 0
		for (size_t i = 0; i < num_iterations; i++)
		{
			if(st.stop_requested()){
				break;
			}
			// if(((double) rand() / (RAND_MAX)) > (1.0/multiplier)){
			// 	busyWait(1000);
			// }

			busyWait(1000);
			if(st.stop_requested()){
				break;
			}
			racer_progress[id]++;
		}
		ss.request_stop(); // technically 2 could request stop at the same time but honestly it doesn't matter that much
		stop = true;
	};

	for (size_t i = 0; i < num_threads; i++)
	{
		// rand int between 1 and 20
		double multiplier = 1 + (rand() % 50);
		threads.push_back(std::jthread(racer, i, ss.get_token(), num_iterations, multiplier));
	}

	// Now all the threads should have started

	// wait for the stop bool to be set
	while (!stop)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(32));
		// clear the screen and print the race board where each racer is represented like a loading bar like Racer 5:      [▮▮▮▮▮-------]
		std::cout << "\033[2J\033[1;1H";
		for (size_t i = 0; i < num_threads; i++)
		{
			printf("Racer %5zu: ", i);

			size_t progress = racer_progress[i];
			print_progress_bar(progress, num_iterations);
			std::cout << std::endl;
		}
	}

	ss.request_stop();
	for (auto &t : threads)
	{
		t.join();
	}

	// clear the screen and print the final
	std::cout << "\033[2J\033[1;1H";
	for (size_t i = 0; i < num_threads; i++)
	{
		printf("Racer %5zu: ", i);
		size_t progress = racer_progress[i];
		print_progress_bar(progress, num_iterations);
		std::cout << std::endl;
	}

	// sort the racers by progress
	std::vector<size_t> sorted_racers(num_threads);
	std::iota(sorted_racers.begin(), sorted_racers.end(), 0);
	std::sort(sorted_racers.begin(), sorted_racers.end(), [](size_t a, size_t b) {
		return racer_progress[a] > racer_progress[b];
	});

	// print the final results
	std::cout << "Final Results" << std::endl;
	for (size_t i = 0; i < num_threads; i++)
	{
		printf("Racer %5zu: %5zu\n", sorted_racers[i], racer_progress[sorted_racers[i]]);
	}
}