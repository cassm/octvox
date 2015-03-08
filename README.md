# octvox
A C++ voxel geometry engine which is concurrency friendly and has a functional (side effect free) API.

## purpose

To provide a fast implementation of a set of basic volumetric geometry operations.

The operations are currently tailored to CAM, but should be applicable to many volumetric modelling applications.

## API

Almost all operations are on Octrees, and most return new Octrees.

Octrees are immutable (and efficiently implemented with data sharing managed by smart pointers). All operations are lock
free and thread safe.
 
A current implementation supports binary space - either filled or empty, but could easily be 
generalised to support arbitrary data per Voxel.  

## Datastructures

The fundamental datastructure is implemented by Octree, and is a hybrid of an octree 
(http://en.wikipedia.org/wiki/Octree) and a 3D array of voxels. It is analogous with a deque (which is a hybrid of 
 an array and a list, effectively a list of small arrays).
 
octvox::Octree uses an octree to decompose space down to a preset size (eg: 64^3 voxels), and then stores a 3D voxel 
array of that size at each 
leaf node. This is intended to provide an optimal mix properties: space and time efficient representation and 
manipulation of sparse 
volumes because of the octree, and similarly efficient representation of dense complex volumes from the voxel arrays 
stored at the leaf nodes.
  
While the asymptotic worst case runtime of operations on octvox::Octree will be no better than the worst of a 
conventional octree and a voxel array, in practice, for many real world tasks, the runtime and size should be close to,
or even better than, the best of them.  

## Status: Pre-Alpha

Very much a WIP at present.
 
The list of operations needs to be filled out, the functionality needs to be packaged as a library, and Octree itself 
needs to be implemented.