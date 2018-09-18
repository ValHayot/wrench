/**
 * Copyright (c) 2017-2018. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <wrench-dev.h>
#include <wrench/simgrid_S4U_util/S4U_Mailbox.h>
#include <wrench/simulation/SimulationMessage.h>
#include "services/compute/standard_job_executor/StandardJobExecutorMessage.h"
#include <gtest/gtest.h>
#include <wrench/services/compute/batch/BatchService.h>
#include <wrench/services/compute/batch/BatchServiceMessage.h>
#include "wrench/workflow/job/PilotJob.h"

#include "../../include/TestWithFork.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(service_restart_test, "Log category for ServiceRestart");

class ServiceRestartTest : public ::testing::Test {

public:
    wrench::ComputeService *compute_service = nullptr;
    wrench::Simulation *simulation;

    void do_ComputeServiceRestart_test();

protected:
    ServiceRestartTest() {

      // Create the simplest workflow
      workflow = std::unique_ptr<wrench::Workflow>(new wrench::Workflow());

      // up from 0 to 100, down from 100 to 200, up from 200 to 300, etc.
      std::string trace_file_content = "PERIODICITY 100\n"
              " 0 1\n"
              " 100 0";

      FILE *trace_file = fopen(trace_file_path.c_str(), "w");
      fprintf(trace_file, "%s", trace_file_content.c_str());
      fclose(trace_file);

      // Create a four-host 10-core platform file
      std::string xml = "<?xml version='1.0'?>"
                                "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">"
                                "<platform version=\"4.1\"> "
                                "   <zone id=\"AS0\" routing=\"Full\"> "
                                "       <host id=\"Host1\" speed=\"1f\" state_file=\""+trace_file_name+"\"  core=\"1\"/> "
                                "       <host id=\"Host2\" speed=\"1f\" core=\"1\"/> "
                                "       <link id=\"1\" bandwidth=\"50000GBps\" latency=\"0us\"/>"
                                "       <route src=\"Host1\" dst=\"Host2\"> <link_ctn id=\"1\"/> </route>"
                                "   </zone> "
                                "</platform>";
      FILE *platform_file = fopen(platform_file_path.c_str(), "w");
      fprintf(platform_file, "%s", xml.c_str());
      fclose(platform_file);

    }

    std::string platform_file_path = "/tmp/platform.xml";
    std::string trace_file_name = "host.trace";
    std::string trace_file_path = "/tmp/"+trace_file_name;
    std::unique_ptr<wrench::Workflow> workflow;

};

/**********************************************************************/
/**             ComputeServiceUsedAfterRestart                       **/
/**********************************************************************/

class ComputeServiceUsedAfterRestartWMS : public wrench::WMS {

public:
    ComputeServiceUsedAfterRestartWMS(ServiceRestartTest *test,
                                 const std::set<wrench::ComputeService *> &compute_services,
                                 std::string hostname) :
            wrench::WMS(nullptr, nullptr, compute_services, {}, {}, nullptr, hostname, "test") {
      this->test = test;
    }

private:

    ServiceRestartTest *test;

    int main() {
      // Create a job manager
      std::shared_ptr<wrench::JobManager> job_manager = this->createJobManager();

      /** Submit a first job **/
      {

        WRENCH_INFO("Submitting job #1");
        // Create a sequential task that lasts one second and requires 1 cores
        wrench::WorkflowTask *task = this->getWorkflow()->addTask("task1", 1, 1, 1, 1.0, 0);

        // Create a StandardJob
        wrench::StandardJob *job = job_manager->createStandardJob(
                {task},
                {},
                {},
                {},
                {});

        // Submit the job for execution, I should get a job completion event
        job_manager->submitJob(job, this->test->compute_service);

        // Wait for a workflow execution event
        std::unique_ptr<wrench::WorkflowExecutionEvent> event;
        try {
          event = this->getWorkflow()->waitForNextExecutionEvent();
        } catch (wrench::WorkflowExecutionException &e) {
          throw std::runtime_error("Error while getting and execution event: " + e.getCause()->toString());
        }
        switch (event->type) {
          case wrench::WorkflowExecutionEvent::STANDARD_JOB_COMPLETION: {

            break;
          }
          default: {
            throw std::runtime_error("Unexpected workflow execution event: " + std::to_string((int) (event->type)));
          }
        }
      }

      /** Sleep long enough that the compute service will have had time to restart **/
      this->simulation->sleep(250);


      /** Submit a second job **/
      {
        WRENCH_INFO("Submitting job #2");
        // Create a sequential task that lasts one second and requires 1 cores
        wrench::WorkflowTask *task = this->getWorkflow()->addTask("task2", 1, 1, 1, 1.0, 0);

        // Create a StandardJob
        wrench::StandardJob *job = job_manager->createStandardJob(
                {task},
                {},
                {},
                {},
                {});

        // Submit the job for execution, I should get a job completion event
        job_manager->submitJob(job, this->test->compute_service);

        // Wait for a workflow execution event
        std::unique_ptr<wrench::WorkflowExecutionEvent> event;
        try {
          event = this->getWorkflow()->waitForNextExecutionEvent();
        } catch (wrench::WorkflowExecutionException &e) {
          throw std::runtime_error("Error while getting and execution event: " + e.getCause()->toString());
        }
        switch (event->type) {
          case wrench::WorkflowExecutionEvent::STANDARD_JOB_COMPLETION: {

            break;
          }
          default: {
            throw std::runtime_error("Unexpected workflow execution event: " + std::to_string((int) (event->type)));
          }
        }
      }

      return 0;
    }
};

TEST_F(ServiceRestartTest, ComputeServiceUsedAfterRestart) {
  DO_TEST_WITH_FORK(do_ComputeServiceRestart_test);
}


void ServiceRestartTest::do_ComputeServiceRestart_test() {


  // Create and initialize a simulation
  auto simulation = new wrench::Simulation();
  int argc = 1;
  auto argv = (char **) calloc(1, sizeof(char *));
  argv[0] = strdup("failure_traces_test");

  ASSERT_NO_THROW(simulation->init(&argc, argv));

  // Setting up the platform
  ASSERT_NO_THROW(simulation->instantiatePlatform(platform_file_path));

  // Get a hostname
  std::string hostname = simulation->getHostnameList()[0];

  std::string wms_hostname = simulation->getHostnameList()[1];

  // Create a Compute Service
  ASSERT_NO_THROW(compute_service = simulation->add(
          new wrench::MultihostMulticoreComputeService(hostname,
                                                       {std::make_tuple(hostname, wrench::ComputeService::ALL_CORES,
                                                                        wrench::ComputeService::ALL_RAM)},
                                                       0, {})));

  simulation->add(new wrench::FileRegistryService(hostname));

  // Create a WMS
  wrench::WMS *wms = nullptr;
  ASSERT_NO_THROW(wms = simulation->add(
          new ComputeServiceUsedAfterRestartWMS(
                  this, {compute_service}, wms_hostname)));

  ASSERT_NO_THROW(wms->addWorkflow(std::move(workflow.get())));

  // Running a "run a single task" simulation
  // Note that in these tests the WMS creates workflow tasks, which a user would
  // of course not be likely to do
  ASSERT_NO_THROW(simulation->launch());

  delete simulation;

  free(argv[0]);
  free(argv);
}

