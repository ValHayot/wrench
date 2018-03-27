/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef WRENCH_NETWORKCONNECTION_H
#define WRENCH_NETWORKCONNECTION_H


#include <string>
#include <wrench/simgrid_S4U_util/S4U_PendingCommunication.h>
#include <wrench/workflow/execution_events/FailureCause.h>

namespace wrench {

    /***********************/
    /** \cond INTERNAL     */
    /***********************/

    class WorkflowFile;

    /**
     * @brief A helper class that implements a network connection abstraction to
     *        be used by a service, e.g., that needs to limit its number of concurrent
     *        connections
     */
    class NetworkConnection {

    public:

        static constexpr unsigned char INCOMING_DATA = 0;
        static constexpr unsigned char OUTGOING_DATA = 1;
        static constexpr unsigned char INCOMING_CONTROL = 2;

        NetworkConnection(int type, WorkflowFile* file, std::string mailbox, std::string ack_mailbox);
        bool start();
        bool hasFailed();
        std::unique_ptr<SimulationMessage> getMessage();

        /** @brief: the connection type */
        int type;
        /** @brief: the file (for a DATA connection) */
        WorkflowFile *file;
        /** @brief: the mailbox for this connection */
        std::string mailbox;
        /** @brief The low-level pending communication */
        std::unique_ptr<S4U_PendingCommunication> comm;
        /** @brief The failure cause, in case the communication fails */
        std::shared_ptr<FailureCause> failure_cause;
        /** @brief: the mailbox to which to send an ack when this connection completes/fails */
        std::string ack_mailbox;
    };

    /***********************/
    /** \endcond           */
    /***********************/

};


#endif //WRENCH_NETWORKCONNECTION_H
