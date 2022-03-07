# cN Tree

The cN tree is a 2-level tree comprised of a root and leaves. Leaf nodes each
contain an exclusive range of keys and their associated values.

## Properties

### Tree

- `fanout`: Number of leaf nodes within the tree.

Fanout must be limited in order to prevent the tiny kvset problem from becoming
unwieldy.

### Node

- `id`: Node ID.
- `num_kvsets`: Number of kvsets contained with the node.
- `kvsets`: List of kvsets.
- `size`: Size of the node.

#### Root

No root specific properties.

#### Leaf

- `min_key`: Minimum key within the range of the node.
- `max_key`: Maximum key within the range of the node.
- `ref_cnt`: Reference count of the node.

Together `min_key` and `max_key` are the edge keys of the node.

## Edge Map

The edge map is the authority on leaf nodes. It knows what nodes contain what
ranges of keys. In order to access a leaf node, one must ask the edge map for a
reference.

## CNDB

The cndb is the authority on tree shape.

## Splitting Nodes

When a node has grown larger than the configured node size, we will initiate a
split. At this point we need to signify to CNDB that we are starting a leaf
split. After taking a reference to the node, we must determine the edge keys for
the new nodes. The left node after the split will have the `min_key` of the
original node. The right node after the split will have the `max_key` of the
original node. The midpoint is the middle of the original node's keyspace.

At this point, we can construct the two new nodes. The two new nodes will exist
in a void until they are ready to be committed to the edge map, but we will be
building them with records along the way in cndb.

In order to split, iterate over all kvsets. Initially, we can check to see if a
kvset is contained entirely within either of the left node or the right node.
If so, move it into the respective node. If not, split the kvset on the midpoint
in in order to create 2 new kvsets in the new nodes, one for each of the nodes.

## Joining Nodes

Joining nodes is a much simpler operation than splitting. In order to split,
we first have to find a sequence of 2 or more nodes whose combined sizes are
less than or equal to the configured node size.

Once this sequence has been identified, we must take references to the nodes. We
take the `min_key` of the new joined node from left-most node in our sequence,
and then we take the `max_key` from the right-most node in our sequence. Given
that all the kvsets within each of the nodes, is within the range of keys
of the new nodes, we can simple move all the kvsets by writing CNDB entries.

## Tree Shape Nearing Max Node Size and Max Fanout

The fanout of the tree is in keeps the tree from growing to infinite leaf nodes.
The reason to keep the fanout is limited is to keep the tiny kvset problem in
check.

Unfortunately, this requires that the cN tree have special behavior as it
reaches max fanout and max node size. Given a fanout `F` and a node size `Z`,
the largest amount of data that could be stored in the leaf nodes is `F*Z`.
Ideally the cN tree should grow forever.

In order to achieve this, the following is done. Trees have a concept of
generations. When a tree is created, the generation (`G`) is 0. The max leaf
node size becomes `(1 << G) * Z`. The generation of a tree increments when we
must increase max node size or cross the fanout threshold. A tree is constantly
moving. At any point in time there are joins (`J`) and splits (`S`) happening
simultaneously. Say the tree stopped ingesting data. Given an `N0` which
represents the current number of nodes, `N1`, the number of nodes after all
ongoing joins and splits finish, can be calculated as `N1 = N0 + S - J`. If at
any point, N1 becomes equal to fanout, it is time to increment the generation of
the tree. In order to keep the tree from continuously hitting this limit, we
must queue up a series of join operations to reclaim leaf node space.

<!--
TODO: How many JOIN operations should we queue up? I would hate to see this
become a garbage collector pause-like situation.
-->
