/* SPDX-FileCopyrightText: 2005 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "node_shader_util.hh"

namespace blender::nodes::node_shader_fresnel_cc {

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Float>("IOR").default_value(1.45f).min(0.0f).max(1000.0f);
  b.add_input<decl::Vector>("Normal").hide_value();
  b.add_output<decl::Float>("Fac");
}

static int node_shader_gpu_fresnel(GPUMaterial *mat,
                                   bNode *node,
                                   bNodeExecData * /*execdata*/,
                                   GPUNodeStack *in,
                                   GPUNodeStack *out)
{
  if (!in[1].link) {
    GPU_link(mat, "world_normals_get", &in[1].link);
  }

  return GPU_stack_link(mat, node, "node_fresnel", in, out);
}

NODE_SHADER_MATERIALX_BEGIN
#ifdef WITH_MATERIALX
{
  /* TODO: some outputs expected be implemented within the next iteration (see nodedef
   * <artistic_ior>) */
  return get_input_value("IOR", NodeItem::Type::Float);
}
#endif
NODE_SHADER_MATERIALX_END

}  // namespace blender::nodes::node_shader_fresnel_cc

/* node type definition */
void register_node_type_sh_fresnel()
{
  namespace file_ns = blender::nodes::node_shader_fresnel_cc;

  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_FRESNEL, "Fresnel", NODE_CLASS_INPUT);
  ntype.declare = file_ns::node_declare;
  ntype.gpu_fn = file_ns::node_shader_gpu_fresnel;
  ntype.materialx_fn = file_ns::node_shader_materialx;

  nodeRegisterType(&ntype);
}
