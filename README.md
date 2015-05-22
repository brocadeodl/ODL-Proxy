ODL-Proxy
=========
Welcome to ODL Proxy
 
This ODL Proxy manages one or more Ethernet Fabrics in a seamless fashion abstracting the underlying fabric and switch specifics.

 
LICENSING
---------
 
The content provided here is licensed under the terms of Apache-2.0 license
http://www.apache.org/licenses/LICENSE-2.0.html

Overview
--------

The primary language is used for this project is ANSI C++.

ODL-Proxy source code is organized into two top level directories

1. **Wave**
2. **Dcm**
3. **Yang**

<H3>Wave</H3>
------------

Wave is a genereal purpose Applciation Development Framework written in C++.  Following are some of the capabilities provided by this framework.

* Built-in Data Types
* Multi Threading
* Multi/Single Master Clustering
* Object Relational Mapping
* Distributed Persistence
* High Availaiblity (Inter Chassis and Intra Chassis)
* Multi Node communication
* ... and many more *(please stay tuned for more information)*

It also supports interfacing with Multipe Noth Bound Itnerfaces.  It currently uses YANG as the Noth Bound interface modeling language.  So, in general, applciations written on Wave can provide seamless access via CLI, REST, NETCONF nortbounds among others.  Wave Also provides its own Client Itnerface mechanism to allow access to the applications by clients.  The applications written using the framework model their data using UML in noth bound independent manner.  It strongly supports MVC design pattern.  In general Wave employs various industry standard design patterns.  It also facilitates using design patterns in the applciations developed using this framework.  Wave supports developing features as pluggable modules.  Applications written using Wave Framework can function as full fledged Application servers.

*Please Stay tuned for more information*

<H3>Dcm</H3>
------------

DCM provides Ethernet fabric management capabilities for typical L2/L3 layer features at this time.  These are implemented usiung Wave Framework as plugins.

*Please Stay tuned for more information*

<H3>YANG</H3>
------------

This directory contains the YANG models for accessing the features provided by DCM.  This directory also contains various runtime auxiliary formats of the YANG content required for compiling DCM code base.
 
HOW TO GET STARTED
------------------
 
Currently, Wave layer has dependencies on various other Open Source projects.  Following is the list of various software projects and their versions that are requried for Wave / DCM to be successfully compiled.

* *Please stay tuned for the list of open source dependecies ...*

<H3>How to fetch the code</H3>

Currently the initial code contribution is made into a branch called **genesis**.  Please use this branch to pull the code in git.

<H3>Compiling</H3>

Once the code is obtained, the open source dependencies mulst be compiled and placed in respective places in the directory structure *Pelase stay tuned for more information*  After this gmake can be used to compile the directories in the following order:
1. Wave/source
2. Wave/application
3. Dcm/source
4. Dcm/Application
5. Dcm/TestApp

<H3>Executing</H3>
*Please stay tuned for more information*
