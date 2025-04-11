// NOTE: This isn't quite right when considering descriptors
template <class G>
struct _partition_vertex_id {
  partition_id_t<G> partition_id; //
  vertex_id_t<G>    vertex_id;    // vertex id within the partition\_id
};

template <class G>
using partition_vertex_id_t = _partition_vertex_id<G>;

// Requirements
vertex_iterator_t<G>     ui = ...;
partition_vertex_id_t<G> puid = partition_vertex_id(g,ui);
vertex_iterator_t<G>     vi = find_partition_vertex(g,puid);
assert(vi == ui);
