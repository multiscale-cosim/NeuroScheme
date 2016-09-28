#include "DataManager.h"

namespace neuroscheme
{
  shift::EntitiesWithRelationships DataManager::_entities =
    shift::EntitiesWithRelationships( );

  shift::EntitiesWithRelationships& DataManager::entities( void )
  {
    return _entities;
  }

  void DataManager::loadData( void )
    {
      //Temp function for testing
      // auto relHierarchyChild = ;
      _entities.relationships( )[ "isParentOf" ] =
        new shift::RelationshipOneToN;

      _entities.relationships( )[ "isChildOf" ] =
        new shift::RelationshipOneToOne;

      shift::Entity* entity =
        new neuroscheme::Column(
          100,
          75, 25,
          5, 0,
          10, 5,
          15, 10,
          20, 15,
          25, 20,
          30, 25,
          10.0f, 30.0f,
          30.0f, 50.0f );
      _entities.add( entity );
      std::cout << "Added column gid " << entity->entityGid( ) << std::endl;
      auto colId = entity->entityGid( );

      entity =
        new neuroscheme::MiniColumn(
          100,
          75, 25,
          5, 0,
          10, 5,
          15, 10,
          20, 15,
          25, 20,
          30, 25,
          10.0f, 30.0f,
          30.0f, 50.0f );
      _entities.add( entity );
      std::cout << "Added minicolumn gid " << entity->entityGid( ) << std::endl;
      auto miniCol0Id = entity->entityGid( );

      entity =
        new neuroscheme::MiniColumn(
          100,
          75, 25,
          5, 0,
          10, 5,
          15, 10,
          20, 15,
          25, 20,
          30, 25,
          10.0f, 30.0f,
          30.0f, 50.0f );
      _entities.add( entity );
      std::cout << "Added minicolumn gid " << entity->entityGid( ) << std::endl;
      auto miniCol1Id = entity->entityGid( );

      auto& relParentOf =
        *( _entities.relationships( )[ "isParentOf" ]->asOneToN( ));
      auto& relChildOf =
        *( _entities.relationships( )[ "isChildOf" ]->asOneToOne( ));
      relParentOf[ colId ].insert( miniCol0Id );
      relParentOf[ colId ].insert( miniCol1Id );
      relChildOf[ miniCol0Id ] = colId;
      relChildOf[ miniCol1Id ] = colId;

      for ( unsigned int i = 0; i < 5; i++ )
      {
        entity = new neuroscheme::Neuron(
          i * 2,
          neuroscheme::Neuron::INTERNEURON,
          neuroscheme::Neuron::EXCITATORY,
          10.0f, 30.0f, 30.0f, 50.0f );
        _entities.add( entity );
        auto entityGid = entity->entityGid( );
        relParentOf[ miniCol0Id ].insert( entityGid );
        relChildOf[ entityGid ] = miniCol0Id;

        entity = new neuroscheme::Neuron(
          i * 2 + 1,
          neuroscheme::Neuron::PYRAMIDAL,
          neuroscheme::Neuron::INHIBITORY,
          70.0f, 60.0f, 20.0f, 30.0f );
        _entities.add( entity );
        entityGid = entity->entityGid( );
        relParentOf[ miniCol0Id ].insert( entityGid );
        relChildOf[ entityGid ] = miniCol0Id;

      }

      for ( unsigned int i = 0; i < 5; i++ )
      {
        entity = new neuroscheme::Neuron(
          i * 2,
          neuroscheme::Neuron::INTERNEURON,
          neuroscheme::Neuron::EXCITATORY,
          10.0f, 30.0f, 30.0f, 50.0f );
        _entities.add( entity );
        auto entityGid = entity->entityGid( );
        relParentOf[ miniCol1Id ].insert( entityGid );
        relChildOf[ entityGid ] = miniCol1Id;

        entity = new neuroscheme::Neuron(
          i * 2 + 1,
          neuroscheme::Neuron::PYRAMIDAL,
          neuroscheme::Neuron::INHIBITORY,
          70.0f, 60.0f, 20.0f, 30.0f );
        _entities.add( entity );
        entityGid = entity->entityGid( );
        relParentOf[ miniCol1Id ].insert( entityGid );
        relChildOf[ entityGid ] = miniCol1Id;

      }
    }
} // namespace neuroscheme
