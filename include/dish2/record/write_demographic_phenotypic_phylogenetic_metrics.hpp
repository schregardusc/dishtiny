#pragma once
#ifndef DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE

#include <mutex>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"

#include "../config/cfg.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../introspection/count_dead_cells.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/count_unique_coding_genotypes.hpp"
#include "../introspection/count_unique_module_expression_profiles.hpp"
#include "../introspection/count_unique_module_regulation_profiles.hpp"
#include "../introspection/count_unique_root_ids.hpp"
#include "../introspection/count_unique_stint_root_ids.hpp"
#include "../introspection/get_fraction_apoptosis_request.hpp"
#include "../introspection/get_fraction_fecund_resource_stockpile.hpp"
#include "../introspection/get_fraction_incoming_inter_message.hpp"
#include "../introspection/get_fraction_incoming_intra_message.hpp"
#include "../introspection/get_fraction_nulliparous.hpp"
#include "../introspection/get_fraction_receiving_resource.hpp"
#include "../introspection/get_fraction_resource_receive_resistance.hpp"
#include "../introspection/get_fraction_resource_reserve_request.hpp"
#include "../introspection/get_fraction_resource_send_request.hpp"
#include "../introspection/get_fraction_spawn_arrest.hpp"
#include "../introspection/get_fraction_spawn_request.hpp"
#include "../introspection/get_mean_cell_age.hpp"
#include "../introspection/get_mean_elapsed_insertions_deletions.hpp"
#include "../introspection/get_mean_elapsed_mutation_occurences.hpp"
#include "../introspection/get_mean_elapsed_point_mutations.hpp"
#include "../introspection/get_mean_epoch.hpp"
#include "../introspection/get_mean_generation.hpp"
#include "../introspection/get_mean_genome_compression_ratio.hpp"
#include "../introspection/get_mean_incoming_inter_message_count.hpp"
#include "../introspection/get_mean_incoming_intra_message_count.hpp"
#include "../introspection/get_mean_kin_group_age.hpp"
#include "../introspection/get_mean_module_count.hpp"
#include "../introspection/get_mean_program_length.hpp"
#include "../introspection/get_mean_resource_received.hpp"
#include "../introspection/get_mean_resource_stockpile.hpp"
#include "../introspection/get_mean_spawn_count.hpp"
#include "../introspection/get_population_compression_ratio.hpp"
#include "../introspection/get_prevalent_coding_genotype.hpp"
#include "../introspection/make_causes_of_death_string_histogram.hpp"
#include "../introspection/sum_entire_elapsed_instruction_cycles_for_live_cells.hpp"
#include "../introspection/sum_entire_elapsed_instruction_cycles.hpp"

#include "make_filename/make_demographic_phenotypic_phylogenetic_metrics_filename.hpp"

namespace dish2 {

template< typename Spec >
void write_demographic_phenotypic_phylogenetic_metrics(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  thread_local emp::DataFile file(
    dish2::make_demographic_phenotypic_phylogenetic_metrics_filename(
      thread_idx
    )
  );

  thread_local std::string metric;
  thread_local double value;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

    file.AddVar(metric, "Metric");
    file.AddVar(value, "Value");
    file.AddVar(update, "Update");
    file.PrintHeaderKeys();
  });

  {
    metric = "Mean Current Epoch";
    value = dish2::get_mean_epoch<Spec>( world );
    file.Update();
  }

  {
    metric = "Random Number Generator Seed";
    value = sgpl::tlrand.Get().GetSeed();
    file.Update();
  }

  // PHYLOGENETIC METRICS

  {
    metric = "Number Phylogenetic Roots";
    value = dish2::count_unique_root_ids<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Stint Phylogenetic Roots";
    value = dish2::count_unique_stint_root_ids<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Insertion/Deletion Mutations";
    value = dish2::get_mean_elapsed_insertions_deletions<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Point Mutations";
    value = dish2::get_mean_elapsed_point_mutations<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Mutation Occurences";
    value = dish2::get_mean_elapsed_mutation_occurences<Spec>( world );
    file.Update();
  }

  for (size_t lev{}; lev <= Spec::NLEV; ++lev) {
    metric = emp::to_string("Mean Elapsed Generations Level ", lev);
    value = dish2::get_mean_generation<Spec>( world, lev );
    file.Update();
  }

  // DEMOGRAPHIC METRICS

  {
    metric = "Number Dead Cells";
    value = dish2::count_dead_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Live Cells";
    value = dish2::count_live_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Unique Genotypes";
    value = dish2::count_unique_coding_genotypes<Spec>( world );
    file.Update();
  }

  {
    metric = "Population Compression Ratio";
    value = dish2::get_population_compression_ratio<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Genome Compression Ratio";
    value = dish2::get_mean_genome_compression_ratio<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Program Module Count";
    value = dish2::get_mean_module_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Program Instruction Count";
    value = dish2::get_mean_program_length<Spec>( world );
    file.Update();
  }

  {
    metric = "Prevalent Genotype Quantity";
    value = dish2::get_prevalent_coding_genotype<Spec>( world ).second;
    file.Update();
  }

  {
    metric = "Mean Cell Age";
    value = dish2::get_mean_cell_age<Spec>( world );
    file.Update();
  }

  for (size_t lev{}; lev < Spec::NLEV; ++lev) {
    metric = emp::to_string("Mean Kin Group Age Level ", lev);
    value = dish2::get_mean_kin_group_age<Spec>( world, lev );
    file.Update();
  }


  // PHENOTYPIC METRICS

  {
    metric = "Number Unique Module Regulation Profiles";
    value = dish2::count_unique_module_regulation_profiles<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Unique Module Expression Profiles";
    value = dish2::count_unique_module_expression_profiles<Spec>( world );
    file.Update();
  }


  {
    metric = "Mean Resource Stockpile";
    value = dish2::get_mean_resource_stockpile<Spec>( world );
    file.Update();
  }

  {
    metric = "Fecund Resource Stockpile Fraction";
    value = dish2::get_fraction_fecund_resource_stockpile<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Receiving Fraction";
    value = dish2::get_fraction_receiving_resource<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Resource Received";
    value = dish2::get_mean_resource_received<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Send Request Fraction";
    value = dish2::get_fraction_resource_send_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Reserve Request Fraction";
    value = dish2::get_fraction_resource_reserve_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Receive Resistance Fraction";
    value = dish2::get_fraction_resource_recieve_resistance<Spec>( world );
    file.Update();
  }

  {
    metric = "Spawn Arrest Fraction";
    value = dish2::get_fraction_spawn_arrest<Spec>( world );
    file.Update();
  }

  {
    metric = "Spawn Request Fraction";
    value = dish2::get_fraction_spawn_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Nulliparous Fraction";
    value = dish2::get_fraction_nulliparous<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Spawn Count";
    value = dish2::get_mean_spawn_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Apoptosis Request Fraction";
    value = dish2::get_fraction_apoptosis_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Incoming Intra Message Count";
    value = dish2::get_mean_incoming_intra_message_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Incoming Inter Message Count";
    value = dish2::get_mean_incoming_inter_message_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Incoming Intra Message Fraction";
    value = dish2::get_fraction_incoming_intra_message<Spec>( world );
    file.Update();
  }

  {
    metric = "Incoming Inter Message Fraction";
    value = dish2::get_fraction_incoming_inter_message<Spec>( world );
    file.Update();
  }

  for (
    const auto& [k, v]
    : dish2::make_causes_of_death_string_histogram<dish2::ThreadWorld<Spec>>(
      world
    )
  ) {
    metric = emp::to_string("Num deaths ", k);
    value = v;
    file.Update();
  }

  {
    metric = "Mean Instructions Executed per Cardinal-update";
    const size_t num_cardinal_updates
      = dish2::count_cardinals<Spec>( world ) * world.GetUpdate();
    value = dish2::sum_entire_elapsed_instruction_cycles<Spec>( world )
      / static_cast<double>( num_cardinal_updates );
    file.Update();
  }

  {
    metric = "Num Instructions Executed per Live Cardinal-update";
    const size_t num_live_cardinal_updates
      = dish2::count_live_cardinals<Spec>( world ) * world.GetUpdate();
      value = dish2::sum_entire_elapsed_instruction_cycles<Spec>( world )
        / static_cast<double>( num_live_cardinal_updates );
    file.Update();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE
