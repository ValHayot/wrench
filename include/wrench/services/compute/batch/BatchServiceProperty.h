/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef WRENCH_BATCHSERVICEPROPERTY_H
#define WRENCH_BATCHSERVICEPROPERTY_H

#include "wrench/services/compute/ComputeServiceProperty.h"

namespace wrench {


    /**
     * @brief Configurable properties for a BatchService
     */
    class BatchServiceProperty: public ComputeServiceProperty {

    public:
        /**
         * @brief The overhead to start a thread execution, in seconds
         */
        DECLARE_PROPERTY_NAME(THREAD_STARTUP_OVERHEAD);

        /**
         * @brief The batch scheduling algorithm. Can be:
         *    - If ENABLE_BATSCHED is set to off / not set:
         *      - "FCFS": First Come First Serve
         *    - If ENABLE_BATSCHED is set to on:
         *      - whatever scheduling algorithm is supported by Batsched
         *                  (by default: "easy_bf")
         *
         **/
        DECLARE_PROPERTY_NAME(BATCH_SCHEDULING_ALGORITHM);

        /**
         * @brief The batch queue ordering algorithm. Can be:
         *     - If ENABLE_BATSCHED is set to off / not set: ignored
         *     - If ENABLE_BATSCHED is set to on:
         *       - whatever queue ordering algorithm is supported by Batsched
         *                  (by default: "fcfs")
         */
        DECLARE_PROPERTY_NAME(BATCH_QUEUE_ORDERING_ALGORITHM);

        /**
         * @brief The host selection algorithm. Can be:
         *      - If ENABLE_BATSCHED is set to off or not set: ignored
         *      - If ENABLE_BATSCHED is set to on:
         *          - FIRSTFIT  (default)
         *          - BESTFIT
         *          - ROUNDROBIN
         **/
        DECLARE_PROPERTY_NAME(HOST_SELECTION_ALGORITHM);

        /**
         * @brief Path to a workload trace file to be replayed. The trace file can be
         * be in the SWF format (see http://www.cs.huji.ac.il/labs/parallel/workload/swf.html), in which
         * case it must have extension ".swf", or in the JSON format as used in the BATSIM project
         * (see https://github.com/oar-team/batsim), in which case is must have the ".json" extension).
         * Note that jobs in the trace whose node/host/processor/core requirements exceed the capacity
         * of the batch service will simply be capped at that capacity.
         */
        DECLARE_PROPERTY_NAME(SIMULATED_WORKLOAD_TRACE_FILE);

        /**
         * @brief Path to a to-be-generated Batsim-style CSV trace file (e.g. for b3atch schedule visualization purposes).
         *      - If ENABLE_BATSCHED is set to off or not set: ignored
         *      - If ENABLE_BATSCHED is set to on: The trace file is generated in CVS format as follows:
         *          allocated_processors,consumed_energy,execution_time,finish_time,job_id,metadata,
         *          requested_number_of_processors,requested_time,starting_time,stretch,submission_time,success,
         *          turnaround_time,waiting_time,workload_name

         */
        DECLARE_PROPERTY_NAME(OUTPUT_CSV_JOB_LOG);


        /**
         * @brief Number of seconds that the Batch Scheduler adds to the runtime of each incoming
         *        job. This is something production batch systems do to avoid too aggressive job
         *        terminations. For instance,
         *        if a job says it wants to run for (at most) 60 seconds, the system
         *        will actually assume the job wants to run for (at most) 60 + 5 seconds.
         */
        DECLARE_PROPERTY_NAME(BATCH_RJMS_DELAY);

        /** @brief Simulate computation as just a sleep instead of an actual comput thread. This is for scalability reason,
         *        and only simulation-valid
        *         if one is sure that cores are space shared (i.e., only a single compute thread can ever
        *         run on a core at once). Since space-sharing at the core level is typically the case in batch-scheduled
         *        clusters, this is likely fine. Possible values are "false" (the default) or "true".
        */
        DECLARE_PROPERTY_NAME(SIMULATE_COMPUTATION_AS_SLEEP);

        /** @brief Controls Batsched logging
         *      - If ENABLE_BATSCHED is set to off or not set: ignored
         *      - If ENABLE_BATSCHED is set to on:
         *          - "true": do not show Batsched logging output on the terminal (default)
         *          - "false": show Batsched logging output on the terminal
         */
        DECLARE_PROPERTY_NAME(BATSCHED_LOGGING_MUTED);

        /** @brief Controls Batsched node allocation policy
        *      - If ENABLE_BATSCHED is set to off or not set: ignored
        *      - If ENABLE_BATSCHED is set to on:
        *          - "false": do not enforce contiguous nodes for allocations (default)
        *          - "true": enforce contiguous nodes for allocations (note that not all algorithms
        *            implemented by batsched support contiguous allocations, so this option may
        *            have no effect in some cases).
        */
        DECLARE_PROPERTY_NAME(BATSCHED_CONTIGUOUS_ALLOCATION);

        /***********************/
        /** \cond INTERNAL     */
        /***********************/


        /***********************/
        /** \endcond           */
        /***********************/

    };
}


#endif //WRENCH_BATCHSERVICEPROPERTY_H
