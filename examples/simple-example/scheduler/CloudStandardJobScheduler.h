/**
 * Copyright (c) 2017-2018. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef WRENCH_CLOUDSCHEDULER_H
#define WRENCH_CLOUDSCHEDULER_H

#include <wrench-dev.h>

namespace wrench {

    class Simulation;

    /**
     * @brief A cloud Scheduler
     */
    class CloudStandardJobScheduler : public StandardJobScheduler {


    public:
        CloudStandardJobScheduler(StorageService *default_storage_service) :
                default_storage_service(default_storage_service) {}

        /***********************/
        /** \cond DEVELOPER    */
        /***********************/

        void scheduleTasks(const std::set<wrench::ComputeService *> &compute_services,
                           const std::vector<wrench::WorkflowTask *> &tasks);

        /***********************/
        /** \endcond           */
        /***********************/

    private:
        std::vector<std::string> execution_hosts;
        std::map<std::string, std::vector<std::string>> vm_list;
        Simulation *simulation;
        StorageService *default_storage_service;
    };
}

#endif //WRENCH_CLOUDSCHEDULER_H
