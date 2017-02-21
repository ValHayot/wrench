//
// Created by Henri Casanova on 2/18/17.
//

#ifndef WRENCH_WORKFLOWTASK_H
#define WRENCH_WORKFLOWTASK_H

#include <map>
#include <lemon/list_graph.h>

#include "WorkflowFile.h"

using namespace lemon;

namespace WRENCH {



		/* Task meta-data class */
		class WorkflowTask {

				friend class Workflow;

				/* Task-state enum */
				enum State {
						READY,
						NOT_READY,
						SCHEDULED,
						RUNNING,
						COMPLETED,
						FAILED
				};

		public:
				std::string id;
				double execution_time; 			// in seconds
				int number_of_processors;		// currently vague: cores? nodes?
				State state;

		private:
				std::shared_ptr<ListDigraph> DAG;  // pointer to the underlying DAG
				ListDigraph::Node DAG_node; // pointer to the underlying DAG node
				std::map<std::string, std::shared_ptr<WorkflowFile>> output_files;  // List of output files
				std::map<std::string, std::shared_ptr<WorkflowFile>> input_files;   // List of input files


		private:
				// Private constructor
				WorkflowTask(const std::string id, const double t, const int n);

				void addInputFile(std::shared_ptr<WorkflowFile> f);
				void addOutputFile(std::shared_ptr<WorkflowFile> f);
				void addFileToMap(std::map<std::string, std::shared_ptr<WorkflowFile>> map, std::shared_ptr<WorkflowFile> f);

		public:
				int getNumberOfChildren();
				int getNumberOfParents();
				WorkflowTask::State getState();




		};

};


#endif //WRENCH_WORKFLOWTASK_H
