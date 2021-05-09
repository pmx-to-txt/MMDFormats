#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/SoftBody.h"

#include "pmx2txt/parser/pmx/Setting.h"

pmx::SoftBody::SoftBody(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, soft_body_name()
	, soft_body_english_name()
	, shape(0)
	, target_material(0)
	, group(0)
	, mask(0)
	, flag(pmx::SoftBodyFlag::BLink)
	, blink_distance(0)
	, cluster_count(0)
	, mass(0.0)
	, collisioni_margin(0.0)
	, aero_model(0)
	, VCF(0.0f)
	, DP(0.0f)
	, DG(0.0f)
	, LF(0.0f)
	, PR(0.0f)
	, VC(0.0f)
	, DF(0.0f)
	, MT(0.0f)
	, CHR(0.0f)
	, KHR(0.0f)
	, SHR(0.0f)
	, AHR(0.0f)
	, SRHR_CL(0.0f)
	, SKHR_CL(0.0f)
	, SSHR_CL(0.0f)
	, SR_SPLT_CL(0.0f)
	, SK_SPLT_CL(0.0f)
	, SS_SPLT_CL(0.0f)
	, V_IT(0)
	, P_IT(0)
	, D_IT(0)
	, C_IT(0)
	, LST(0.0f)
	, AST(0.0f)
	, VST(0.0f)
	, anchor_count(0)
	, anchors()
	, pin_vertex_count(0)
	, pin_vertices()
{}

void pmx::SoftBody::parse(std::istream& stream)
{
	// 未実装
	throw std::runtime_error("Not Implemented Exception");
}

std::size_t pmx::SoftBody::dump(std::ostream& stream) const
{
	throw std::runtime_error("Not Implemented Exception");
}
