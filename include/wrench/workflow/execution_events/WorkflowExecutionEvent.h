/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */


#ifndef WRENCH_WORKFLOWEXECUTIONEVENT_H
#define WRENCH_WORKFLOWEXECUTIONEVENT_H

#include <string>
#include "FailureCause.h"

/***********************/
/** \cond DEVELOPER    */
/***********************/

namespace wrench {

    class WorkflowTask;

    class WorkflowFile;

    class StandardJob;

    class PilotJob;

    class ComputeService;

    class StorageService;

    class FileRegistryService;

    class FileRegistryService;

    /**
     * @brief A class to represent the various execution events that
     * are relevant to the execution of a workflow.
     */
    class WorkflowExecutionEvent {

    public:

        /** @brief Workflow execution event types */
        enum EventType {
            /** @brief An error type */
                    UNDEFINED,
            /** @brief A standard job successfully completed */
                    STANDARD_JOB_COMPLETION,
            /** @brief A standard job failed */
                    STANDARD_JOB_FAILURE,
            /** @brief A pilot job started */
                    PILOT_JOB_START,
            /** @brief A pilot job expired */
                    PILOT_JOB_EXPIRATION,
            /** @brief A file copy operation completed */
                    FILE_COPY_COMPLETION,
            /** @brief A file copy operation failed */
                    FILE_COPY_FAILURE,
        };

        /** @brief The event type */
        WorkflowExecutionEvent::EventType type;


        /***********************/
        /** \cond INTERNAL     */
        /***********************/
        static std::unique_ptr<WorkflowExecutionEvent> waitForNextExecutionEvent(std::string);
        virtual ~WorkflowExecutionEvent() = default;

    protected:

        /**
         * @brief Constructor
         *
         * @param type: event type
         */
        WorkflowExecutionEvent(EventType type) : type(type) {}
        /***********************/
        /** \endcond           */
        /***********************/

    };


    /**
     * @brief A "standard job has completed" WorkflowExecutionEvent
     */
    class StandardJobCompletedEvent : public WorkflowExecutionEvent {


    private:

        friend class WorkflowExecutionEvent;
        /**
         * @brief Constructor
         * @param standard_job: a standard job
         * @param compute_service: a compute service
         */
        StandardJobCompletedEvent(StandardJob *standard_job,
                                  ComputeService *compute_service)
                : WorkflowExecutionEvent(STANDARD_JOB_COMPLETION),
                  standard_job(standard_job), compute_service(compute_service) {}
    public:

        /** @brief The standard job that has completed */
        StandardJob *standard_job;
        /** @brief The compute service on which the standard job has completed */
        ComputeService *compute_service;
    };

    /**
     * @brief A "standard job has failed" WorkflowExecutionEvent
     */
    class StandardJobFailedEvent : public WorkflowExecutionEvent {

    private:

        friend class WorkflowExecutionEvent;

        /**
         * @brief Constructor
         * @param standard_job: a standard job
         * @param compute_service: a compute service
         * @param failure_cause: a failure_cause
         */
        StandardJobFailedEvent(StandardJob *standard_job,
                               ComputeService *compute_service,
                               std::shared_ptr<FailureCause> failure_cause)
                : WorkflowExecutionEvent(STANDARD_JOB_FAILURE),
                  standard_job(standard_job),
                  compute_service(compute_service),
                  failure_cause(failure_cause) {}

    public:

        /** @brief The standard job that has failed */
        StandardJob *standard_job;
        /** @brief The compute service on which the job has failed */
        ComputeService *compute_service;
        /** @brief The cause of the failure */
        std::shared_ptr<FailureCause> failure_cause;
    };


    /**
     * @brief A "pilot job has started" WorkflowExecutionEvent
     */
    class PilotJobStartedEvent : public WorkflowExecutionEvent {

    private:

        friend class WorkflowExecutionEvent;

        /**
         * @brief Constructor
         * @param pilot_job: a pilot job
         * @param compute_service: a compute service
         */
        PilotJobStartedEvent(PilotJob *pilot_job,
                             ComputeService *compute_service)
                : WorkflowExecutionEvent(PILOT_JOB_START),
                  pilot_job(pilot_job), compute_service(compute_service) {}

    public:
        /** @brief The pilot job that has started */
        PilotJob *pilot_job;
        /** @brief The compute service on which the pilot job has started */
        ComputeService *compute_service;
    };

    /**
     * @brief A "pilot job has expired" WorkflowExecutionEvent
     */
    class PilotJobExpiredEvent : public WorkflowExecutionEvent {

    private:

        friend class WorkflowExecutionEvent;
        /**
         * @brief Constructor
         * @param pilot_job: a pilot job
         * @param compute_service: a compute service
         */
        PilotJobExpiredEvent(PilotJob *pilot_job,
                             ComputeService *compute_service)
                : WorkflowExecutionEvent(PILOT_JOB_EXPIRATION),
                  pilot_job(pilot_job), compute_service(compute_service) {}

    public:

        /** @brief The pilot job that has expired */
        PilotJob *pilot_job;
        /** @brief The compute service on which the pilot job has expired */
        ComputeService *compute_service;
    };

    /**
     * @brief A "file copy has completed" WorkflowExecutionEvent
     */
    class FileCopyCompletedEvent : public WorkflowExecutionEvent {

    private:

        friend class WorkflowExecutionEvent;
        /**
         * @brief Constructor
         * @param file: a workflow file
         * @param storage_service: a storage service
         * @param file_registry_service: a file registry service
         * @param file_registry_service_updated: whether the file registry service has been updated
         */
        FileCopyCompletedEvent(WorkflowFile *file,
                               StorageService *storage_service,
                               FileRegistryService *file_registry_service,
                               bool file_registry_service_updated)
                : WorkflowExecutionEvent(FILE_COPY_COMPLETION),
                  file(file), storage_service(storage_service),
                  file_registry_service(file_registry_service),
                  file_registry_service_updated(file_registry_service_updated) {}

    public:
        /** @brief The workflow file that has successfully been copied */
        WorkflowFile *file;
        /** @brief The storage service to which the file has been copied */
        StorageService *storage_service;
        /** @brief The file registry service that was supposed to be updated (or nullptr if none) */
        FileRegistryService *file_registry_service;
        /** @brief Whether the file registry service (if any) has been successfully updated */
        bool file_registry_service_updated;
    };


    /**
     * @brief A "file copy has failed" WorkflowExecutionEvent
     */
    class FileCopyFailedEvent : public WorkflowExecutionEvent {

    private:

        friend class WorkflowExecutionEvent;
        /**
         * @brief Constructor
         * @param file: a workflow file
         * @param storage_service: a storage service
         * @param failure_cause: a failure cause
         */
        FileCopyFailedEvent(WorkflowFile *file,
                            StorageService *storage_service,
                            std::shared_ptr<FailureCause> failure_cause
        )
                : WorkflowExecutionEvent(FILE_COPY_FAILURE),
                  file(file), storage_service(storage_service),
                  failure_cause(failure_cause) {}

    public:

        /** @brief The workflow file that has failed to be copied */
        WorkflowFile *file;
        /** @brief The storage service on which it was supposed to be copied */
        StorageService *storage_service;
        /** @brief The cause of the failure */
        std::shared_ptr<FailureCause> failure_cause;

    };

};

/***********************/
/** \endcond           */
/***********************/



#endif //WRENCH_WORKFLOWEXECUTIONEVENT_H
