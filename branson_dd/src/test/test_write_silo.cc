/*
  Author: Alex Long
  Date: 4/15/2016
  Name: test_write_silo.cc
*/

#include <iostream>
#include <string>
#include <vector>

#include "../mesh.h"
#include "../mpi_types.h"
#include "../write_silo.h"
#include "../decompose_mesh.h"
#include "testing_functions.h"

int main (int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  
  int rank, n_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);

  MPI_Types *mpi_types = new MPI_Types();

  using std::cout;
  using std::endl;
  using std::string;

  int nfail = 0;

  // Test that a simple mesh (one division in each dimension) is constructed
  // correctly from the input file (simple_input.xml) and that the write
  // silo function exits without error
  {
    string filename("simple_input.xml");
    Input *input = new Input(filename);

    Mesh *mesh = new Mesh(input, mpi_types, rank, n_rank);

    bool silo_write_pass = true;

    decompose_mesh(mesh, mpi_types);

    // get fake vector of mesh requests 
    std::vector<uint32_t> n_requests(mesh->get_global_num_cells(),0);
    
    double time = 0.0;
    int step = 0;
    write_silo(mesh, time, step, rank, n_rank, n_requests); 

    if (silo_write_pass) cout<<"TEST PASSED: writing simple mesh silo file"<<endl;
    else { 
      cout<<"TEST FAILED:  writing silo file"<<endl; 
      nfail++;
    }
    delete mesh;
    delete input;
  }

  // Test that a simple mesh (one division in each dimension) is constructed
  // correctly from the input file (simple_input.xml) and that each cell 
  // is assigned the correct region
  {
    string filename("three_region_mesh_input.xml");
    Input *input = new Input(filename);

    Mesh *mesh = new Mesh(input, mpi_types, rank, n_rank);

    bool three_reg_silo_write_pass = true;

    decompose_mesh(mesh, mpi_types);

    // get fake vector of mesh requests 
    std::vector<uint32_t> n_requests(mesh->get_global_num_cells(),0);

    double time = 2.0;
    int step = 1;
    write_silo(mesh, time, step, rank, n_rank, n_requests);

    if (three_reg_silo_write_pass) {
      cout<<"TEST PASSED: writing three region mesh silo file"<<endl;
    }
    else { 
      cout<<"TEST FAILED:  writing silo file"<<endl; 
      nfail++;
    }
    delete mesh;
    delete input;
  }

  delete mpi_types;

  MPI_Finalize();

  return nfail;
}
