/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef APPLICATIONSERVICEINTERFACE_H
#define APPLICATIONSERVICEINTERFACE_H

// Name:
//     PrismApplicationServiceMessageHandler
//
// Description:
//     Data type for a function pointer which can be set as a message Handler.  When a Service receives a message, depending on the opcode
//     given to the message it executes a registered Message Handler for that opcode.  The message handler must reply to the message using
//     replyToApplicationService api call.
//
// Parameters:
//       pPayLoad:
//           Pointer to the Pay Load given by the sender Service.  The memory alllocated for the pay load is owned by the framework.
//           The user of this must NOT delete the allocated memory.  If the user requires the contents even after replying to the corresponding
//           message, then the user needs to copy the contents into memory that is owned by the user.
//       payLoadLength:
//           Length of the pay load given by the sender.
//       pPrismContext:
//           Prism Context.  This context must be used when replying to the message.  Please refre to the replyToApplicationService API call.
//
// Return Value:
//     None

typedef void (*PrismApplicationServiceMessageHandler) (void *pPayLoad, const unsigned int payLoadLength, void *pPrismContext);

// Name:
//    PrismApplicationServiceCallback
//
// Description:
//     Data type for a function pointer that can be set as a Callback function when a message is sent out to an Prism application Service.
//     Anytime when a message is sent to a Prism Appliction Service using the send family of API calls, there will be a callback that gets
//     executed upon the arrival of a reply to the originally sent message.  However, there is an exception.  Synchronously sent messages
//     do not result in callbacks.  The API call corresponding to sending synchronous messages does not require a callback to be specified.
//
// Parameters:
//     pOutputPayLoad:
//         Pointer to the output payload sent by the receiver of the message that was sent.  The memory allocated to hold the putput pay load
//         is owned by the framework.  The user must make a copy of the contents of this buffer if accesss to this data is required after
//         exiting from the callback function.  The output payload will be deleted soon after exiting the callback function.
//     outputPayLoadLength:
//         Length of the output pay load sent by the receiver of the message.
//     pApplicationServiceContext:
//         This is the context specified by the sender Prism Application Service when it sent a message using one of the send family of
//         asynchronous API calls.  The framework guarantees that the context is not modified by the framework in any way after it is received
//         in a send family of API calls and before it is given back to the sender via callback execution.
//
// Return Value:
//     None

typedef void (*PrismApplicationServiceCallback) (void *pOutputPayLoad, const unsigned int outputPayLoadLength, void *pApplicationServiceContext);

#ifdef __cplusplus
extern "C"
{
#endif

// Name:
//     registerNewApplicationService
//
// Description:
//     A function to register a new Prism Application Global Service.  This tells Prism that a global Application Service must be created when the Prism
//     initializes.  When Prism is initialized, Prism create one thread per each of the Services that it knows to create.
//     For a detailed discussion on Prism Global and Local services, please refer to Prism Functional Specification.
//
// Parameters:
//     pApplicationServiceName:
//         A pointer to constant array of characters.  This name must be unique.  A duplicate name may result in an assert.
//
// Return Value:
//     An unsinged inerger indicating the identification for the new created service.  This will be greater than 0 if the service is registered
//     successfully.  A value 0 indicates failure.  This identifier will be required to specify the service to operate on in the other API
//     calls.

unsigned int registerNewApplicationService (const char *pApplicationServiceName);

// Name:
//     registerNewApplicationLocalService
//
// Description:
//     A function to register a new Prism Application Local Service.  This tells Prism that a local Application Service must be created when the Prism
//     initializes.  When Prism is initialized, Prism create one thread per each of the Services that it knows to create.
//     For a detailed discussion on Prism Global and Local services, please refer to Prism Functional Specification.
//
// Parameters:
//     pApplicationServiceName:
//         A pointer to constant array of characters.  This name must be unique.  A duplicate name may result in an assert.
//
// Return Value:
//     An unsinged inerger indicating the identification for the new created service.  This will be greater than 0 if the service is registered
//     successfully.  A value 0 indicates failure.  This identifier will be required to specify the service to operate on in the other API
//     calls.

unsigned int registerNewApplicationLocalService (const char *pApplicationLocalServiceName);

// Name:
//     addMessageHandler
//
// Description:
//     A function to setup a mesage handler that needs to be invoked by a Prism Application Service when a message with corresponding opcode
//     arrives.
//
// Parameters:
//     applicationServiceId:
//         The Prism Application Service Id of the Service that is interested in receiveing the message.  Typically this id is returned to the
//         user during the registration of the Application Service.
//     operationCode:
//         The operation code identifying the particular operation (or message) processed by the Application Service.
//     pHandler:
//         Pointer to the function that needs to executed when the message with this opcode arrives at this Application Service.
//         Please refer to description of PrismApplicationServiceMessageHandler for more details.
//
// Return Value:
//     None

void addMessageHandler (const unsigned int applicationServiceId, const unsigned int operationCode, PrismApplicationServiceMessageHandler pHandler);

// Name:
//     sendToApplicationService
//
// Description:
//     A function to send a Request (message) to a Prism Application Service.   When a reply arrives for this request the callback that
//     gets registered as part of this function call will be executed.  IF a NULL callback is specified no callback will be executed.
//     The requestor Service can send any input parameters in payload.  It is assumed that the Sender and the receiver agree upon the
//     input format.  The payload will be given to the receiver via a call to the corresponding message handler that is registred by the
//     receiver.
//
//     This API call can be used to send requests (messages) to Application Services running in the same node as well as Application Services
//     running on a remote node.  The Location Id argument captures the location where the message is to be deleivered.
//
//     This function call send the message asynchronously.  Completion of this function does not mean that the receiver processed the message
//     and responded back to the sender.  It only guarantees that the framework is able to successfully schedule the message to be delivered
//     to the Receiver Application Service.  In case of delivering message to a Service in the local node, it guarantees that the mesage could
//     be successfully queued into the receipient message queues.  In case of delivering to a remote node, it guarantees that the message has
//     has been successfully submitted to the Reporte Transport Service on the local node for remote delivery.
//
// Parameters:
//     sendingApplicationServicePrismServiceId:
//         The Prism Application Service Id of the sender service.  This is different from the Priam Service Id itself.  This value is typically
//         returned to the user during the Application registration.  Framework internal.ly maps this to the corresponding Prism Service Id
//         before routing the message.
//     pPayLoad:
//         The input payload to be sent to the receiver.  This can be deleted/altered after the function call returns.  The framework makes a
//         copy of this buffer before returning.  The user is responsible for deletion of the buffer.
//     payLoadLength:
//         Length of the input payload.
//     receivingApplicationServicePrismServiceId:
//         Receiving Prism Application Service Id.  This is similar to sendingApplicationServicePrismServiceId.
//     prismLocationId:
//         The Location Id of the node to which the request (message) has to be sent.
//     pPrismApplicationServiceCallback:
//         Pointer to a function that can be used as a callback.  If the Callback is NULL, then no callback function will be executed upon the
//         reply arrival.
//     pApplicationContext:
//         An aritrary pointer value that will be given back as part of the reply callback when the reply arrives.  Users can use this field to
//         save state information and retrieve it during the callback execution.
//
// Return Value:
//     A return value of WAVE_MESSAGE_SUCCESS indicates success.  Any other value indicates an error.

unsigned int sendToApplicationService (const unsigned int                              sendingApplicationServicePrismServiceId,
                                                      void                            *pPayLoad,
                                       const unsigned int                              payLoadLength,
                                       const unsigned int                              receivingApplicationServicePrismServiceId,
                                       const unsigned int                              prismLocationId,
                                                      PrismApplicationServiceCallback  pPrismApplicationServiceCallback,
                                                      void                            *pApplicationContext);

// Name:
//   replyToApplicationService
//
// Description:
//     A function to send response to a message that was received by the Prism Application Service.  Every message sender excepts to receive
//     a response to the message that was send except in the case of sending message one way or sending with no callback.  The receiver of the
//     message must reply to the message after processing the message.  The reply must happen irrespective of how the message was sent (even  if
//     it is send with one way or no callback).  This simplifies receiver's responsibility.
//
//     Receiver can optinally specify some output pay load when replying.  The buffer can be modified/altered after this function call returns.
//     The framework makes a copy of te output buffer given during this call.  The ownership of the buffer remains with the user.  User is
//     responsible to delete it after he is done using it.  The memory for the copy made will be managed by the framework.
//
//     User must supply back the context that was given to the user during the executing of corresponding message handler.  User must not have
//     modified this context.
//
// Parameters:
//     pOutputPayLoad:
//         A pointer to the output buffer.  This buffer must not be changed during the execution of the function.  Framework will make a copy
//         of this buffer during the call.  The responsibility of the original buffer remins with the user.
//     outputPayLoadLength:
//         Length of the output payload.
//     pPrismContext:
//         The Prism context that was given as part of the corresponding message handler.  This must not have been altered.
//
// Return Value:
//     None

void replyToApplicationService (void *pOutputPayLoad, const unsigned int outputPayLoadLength, void *pPrismContext);

#ifdef __cplusplus
}
#endif

#endif // APPLICATIONSERVICEINTERFACE_H
