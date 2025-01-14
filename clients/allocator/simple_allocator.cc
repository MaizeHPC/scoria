extern "C" {
#include "scoria.h"
}

#include "shm_allocator.hh"

#include <vector>

int main(int argc, char **argv) {
  // Suppress Compiler Warnings
  (void)argc;
  (void)argv;

  struct client client;
  client.chatty = 0;

  scoria_init(&client);

  std::vector<double, shm_allocator<double>> A(1024);
  std::vector<double, shm_allocator<double>> input(1024);

  for (size_t i = 0; i < 1024; ++i)
    input[i] = (double)(2 * i);

  // Write to Buffer
  printf("Client (%d): Writing array:\n", client.id);

  struct request req1;
  scoria_write(&client, A.data(), 1024, input.data(), NULL, NULL, 0, NONE,
               &req1, NULL);
  scoria_wait_request(&client, &req1);

  // Read from Buffer
  printf("Client(%d): Reading Array:\n", client.id);

  std::vector<double, shm_allocator<double>> output(1024);

  struct request req2;
  scoria_read(&client, A.data(), 1024, output.data(), NULL, NULL, 0, NONE,
              &req2, NULL);
  scoria_wait_request(&client, &req2);

  for (size_t i = 0; i < 1024; ++i)
    printf("%.2f ", output[i]);
  printf("\n");

  struct request quit_req;
  scoria_quit(&client, &quit_req);
  scoria_wait_request(&client, &quit_req);
  scoria_cleanup(&client);

  printf("Exiting...\n");

  return 0;
}
