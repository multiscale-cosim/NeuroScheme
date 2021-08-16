/*
 * Copyright (c) 2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
 *
 * This file is part of NeuroScheme
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include <scoop/scoop.h>
#include <nslib/mappers/VariableMapper.h>
#include <nslib/error.h>
#include <nslib/DataManager.h>
#include <nslib/Loggers.h>
#include "RepresentationCreator.h"
#include <shift_NeuronSuperPop.h>
#include <shift_ConnectsWith.h>
#include "CongenPopRep.h"
#include "NeuronSuperPopRep.h"
#include "ConnectionArrowRep.h"
#include "AutoConnectionArrowRep.h"
#include "NeuronSuperPopItem.h"
#include <algorithm>
#include <shift_AggregatedConnectsWith.h>

namespace nslib
{
  namespace congen
  {

    RepresentationCreator::RepresentationCreator( void )
      : _maxNeuronsPerPopulation( 1 )
      , _maxLevelsPerSuperPop( 1 )
      , _maxAbsoluteWeight( 0.1f )
      , _superPopSeparation( 1.0f / (_maxLevelsPerSuperPop + 1 ))
      , _superPopLevelSeparation( NeuronSuperPopItem::rangeCircle
          * _superPopSeparation)
      , _nbConnectionsToWidth( 0, _maxAbsoluteWeight, 1.0f, 5.0f )
      ,  _neuronsToPercentage( 0, _maxNeuronsPerPopulation, 0.0f, 1.0f )
      , _superPopLevelColorMap( 0.0f, scoop::Color( "#b6d7a8" ),
          1.0f, scoop::Color( "#b075f0" ))
      , _superPopColor( "#b6d7a8" )
    {
      _neuronModelColorMap.setColor(
        shiftgen::NeuronPop::TNeuronModel::iaf_psc_alpha,
        scoop::Color( "#9fc5e8" ));
      _neuronModelColorMap.setColor(
        shiftgen::NeuronPop::TNeuronModel::undefined,
        scoop::Color( "#ea9999" ));
      _neuronModelColorMap.setColor(
        shiftgen::NeuronPop::TNeuronModel::nmm_kuramoto,
        scoop::Color( "#ea0099" ));
      _neuronModelColorMap.setColor(
        shiftgen::NeuronPop::TNeuronModel::nmm_2doscillator,
        scoop::Color( "#0a9909" ));
      _neuronModelColorMap.setColor(
        shiftgen::NeuronPop::TNeuronModel::proxy,
        scoop::Color( "#0a5959" ));

      _stimulatorModelColorMap.setColor(
        shiftgen::Input::TInputType::Pulse_input,
        scoop::Color( "#ddf231" ));
      _stimulatorModelColorMap.setColor(
        shiftgen::Input::TInputType::Random_stim,
        scoop::Color( "#f9f190" ));

      _receptorModelColorMap.setColor(
        shiftgen::Output::TOutputModel::Multimeter,
        scoop::Color( "#d7b5fc" ));
      _receptorModelColorMap.setColor(
        shiftgen::Output::TOutputModel::Voltmeter,
        scoop::Color( "#b075f0" ));
      _receptorModelColorMap.setColor(
        shiftgen::Output::TOutputModel::Spike_detector,
        scoop::Color( "#4c1c7c" ));

    }

    void RepresentationCreator::updateRepresentation(
      const shift::Entity* entity_, shift::Representation* entityRep_
    )
    {
      if ( dynamic_cast< const shiftgen::NeuronPop* >( entity_ ))
      {
        updateNeuronPopRep( entity_, entityRep_ );
      }
      else if(  dynamic_cast< const shiftgen::Input* >( entity_ ))
      {
        updateInputRep( entity_, entityRep_ );
      }
      else if( dynamic_cast< const shiftgen::NeuronSuperPop* >( entity_ ))
      {
        updateSuperPopRep( entity_, entityRep_ );
      }
      else if( dynamic_cast< const shiftgen::Output* >( entity_ ))
      {
        updateOutputRep( entity_, entityRep_ );
      }
    }

    void RepresentationCreator::updateNeuronPopRep(
      const shift::Entity* entity_, shift::Representation* entityRep_ )
    {
      if ( entity_->hasProperty( "Entity name" ))
      {
        entityRep_->setProperty( "Entity name", entity_->getProperty( "Entity name" )
            .value<std::string>( ));
      }
      else
      {
        Loggers::get( )->log( "Expected property Entity name.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "Entity name", " " );
      }
      if ( entity_->hasProperty( "Neuron model" ))
      {
        entityRep_->setProperty(
          "color", _neuronModelColorMap.getColor(
          entity_->getProperty( "Neuron model" )
          .value< shiftgen::NeuronPop::TNeuronModel >( )));
      }
      else
      {
        entityRep_->setProperty( "color", _neuronModelColorMap.getColor(
          shiftgen::NeuronPop::TNeuronModel::undefined ) );
      }
      if ( entity_->hasProperty( "Nb of neurons" ))
      {
        entityRep_->setProperty( "line perc", _neuronsToPercentage.map(
          entity_->getProperty( "Nb of neurons" ).value< uint >( )));
      }
      else
      {
        Loggers::get( )->log( "Expected property Nb of neurons.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "line perc", _neuronsToPercentage.map( 0u ));
      }
    }

    void RepresentationCreator::updateSuperPopRep( const shift::Entity* entity_,
      shift::Representation* entityRep_ )
    {
      entityRep_->setProperty( "color", _superPopColor );
      entityRep_->setProperty( "circles separation", _superPopLevelSeparation );
      entityRep_->setProperty( "circles color separation",
        _superPopSeparation );
      entityRep_->setProperty( "circles color map", _superPopLevelColorMap );

      if ( entity_->hasProperty( "Nb of neurons Mean" ))
      {
        entityRep_->setProperty( "line perc", _neuronsToPercentage.map(
          entity_->getProperty( "Nb of neurons Mean" ).value< uint >( )));
      }
      else
      {
        Loggers::get( )->log( "Expected property Nb of neurons Mean.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "line perc",
          _neuronsToPercentage.map( 0u ));
      }
      if ( entity_->hasProperty( "child depth" ))
      {
        entityRep_->setProperty( "num circles", entity_->getProperty( "child depth" )
          .value<unsigned int>( ));
      }
      else
      {
        Loggers::get( )->log( "Expected property Level.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "num circles", 0u);
      }
      if ( entity_->hasProperty( "Entity name" ))
      {
        entityRep_->setProperty( "Entity name", entity_->getProperty( "Entity name" )
          .value<std::string>( ));
      }
      else
      {
        Loggers::get( )->log( "Expected property Entity name.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "Entity name", " " );
      }
    }

    void RepresentationCreator::updateInputRep(
      const shift::Entity* entity_, shift::Representation* entityRep_ )
    {
      if( entity_->hasProperty( "Input model" ))
      {
        entityRep_->setProperty(
          "color", _stimulatorModelColorMap.getColor(
          entity_->getProperty( "Input type" )
          .value< shiftgen::Input::TInputType >( )));
      }
      else
      {
        entityRep_->setProperty( "color",
          _stimulatorModelColorMap.getColor(
          shiftgen::Input::Random_stim ));
      }
      if ( entity_->hasProperty( "Nb of neurons" ))
      {
        entityRep_->setProperty( "line perc", _neuronsToPercentage.map(
          entity_->getProperty( "Nb of neurons" ).value< uint >( )));
      }
      else
      {
        Loggers::get( )->log( "Expected property Nb of neurons.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "line perc", _neuronsToPercentage.map( 0u ));
      }
      if ( entity_->hasProperty( "Entity name" ))
      {
        entityRep_->setProperty( "Entity name", entity_->getProperty( "Entity name" )
            .value<std::string>( ));
      }
      else
      {
        Loggers::get( )->log( "Expected property Entity name.",
                              LOG_LEVEL_WARNING );
        entityRep_->setProperty( "Entity name", " " );
      }
    }

    void RepresentationCreator::updateOutputRep( const shift::Entity* entity_,
      shift::Representation* entityRep_ )
    {
      if( entity_->hasProperty( "Output model" ))
      {
        entityRep_->setProperty( "color", _receptorModelColorMap.getColor(
          entity_->getProperty( "Output model" )
          .value< shiftgen::Output::TOutputModel >( )));
      }
      else
      {
        entityRep_->setProperty( "color", _receptorModelColorMap.getColor(
          shiftgen::Output::Multimeter ));
      }
      if ( entity_->hasProperty( "Entity name" ))
      {
        entityRep_->setProperty( "Entity name", entity_->
          getProperty( "Entity name" ).value< std::string >( ));
      }
      else
      {
        Loggers::get( )->log( "Expected property Entity name.",
          LOG_LEVEL_WARNING );
        entityRep_->setProperty( "Entity name", " " );
      }
    }

    void RepresentationCreator::create(
      const shift::Entities& entities,
      shift::Representations& representations,
      shift::TEntitiesToReps& entitiesToReps,
      shift::TRepsToEntities& repsToEntities,
      shift::TGidToEntitiesReps& gidsToEntitiesReps,
      bool linkEntitiesToReps,
      bool linkRepsToEntities )
    {
      nslib::Loggers::get( )->log( "create",
        LOG_LEVEL_VERBOSE, NEUROSCHEME_FILE_LINE );


      // scoop::SequentialColorMap neuronTypeColorMapper(
      //   scoop::ColorPalette::colorBrewerSequential(
      //     scoop::ColorPalette::ColorBrewerSequential::PuBu, 6 ),
      //   0.0f, _maxNeuronsPerPopulation );

      for ( const auto entity : entities.vector( ))
      {
        if ( entitiesToReps.find( entity ) != entitiesToReps.end( ))
        {
          for ( const auto rep : entitiesToReps[ entity ] )
            representations.push_back( rep );
          continue;
        }
        shift::Representation* entityRep = nullptr;

        if ( dynamic_cast< shiftgen::NeuronPop* >( entity ))
        {
          entityRep = new CongenPopRep( );
          updateNeuronPopRep( entity, entityRep );
        }
        else if( dynamic_cast< shiftgen::NeuronSuperPop* >( entity ))
        {
          entityRep = new NeuronSuperPopRep( );
          updateSuperPopRep( entity, entityRep );
        }
        else if( dynamic_cast< shiftgen::Input* >( entity ) )
        {
          entityRep = new CongenPopRep( );
          updateInputRep( entity, entityRep );
        }
        else if( dynamic_cast< shiftgen::Output* >( entity ) )
        {
          entityRep = new CongenPopRep( );
          updateOutputRep( entity, entityRep );
        }
        if ( entityRep )
        {
          representations.push_back( entityRep );
          // Link entity and rep
          if ( linkEntitiesToReps )
            entitiesToReps[ entity ].insert( entityRep );
          if ( linkRepsToEntities )
            repsToEntities[ entityRep ].insert( entity );

          gidsToEntitiesReps
            .insert( TripleKey( entity->entityGid( ), entity, entityRep ));
        }
      }
    }

    void RepresentationCreator::generateRelations(
      const shift::Entities& entities,
      const shift::TGidToEntitiesReps& gidsToEntitiesReps,
      shift::TRelatedEntitiesReps& relatedEntitiesReps,
      shift::Representations& relatedEntities,
      shift::RelationshipOneToN* relatedElements )
    {
      for( auto& entity : entities.vector( ))
      {
        auto srcEntityRep = gidsToEntitiesReps.find( entity->entityGid( ));
        if( srcEntityRep == gidsToEntitiesReps.end( ))
          continue;

        auto entityRelations = relatedElements->find( entity->entityGid( ));

        if( entityRelations == relatedElements->end( ))
          continue;

        for( auto& other : entities.vector( ))
        {
          auto otherRep = gidsToEntitiesReps.find( other->entityGid( ));
          if( otherRep == gidsToEntitiesReps.end( ))
            continue;

          auto numberOfConnections =
            entityRelations->second.count( other->entityGid( ));

          if( numberOfConnections == 0 )
            continue;

          // TODO: Change to equal_range whenever multiple relationships between
          // the same elements are imported. Then, create a loop to iterate
          // over the given results and create a new one if not found.
          auto combinedKey = std::make_pair( entity->entityGid( ),
            other->entityGid( ));
          auto alreadyConnected =
            relatedEntitiesReps.find( combinedKey );

          if( alreadyConnected == relatedEntitiesReps.end( ))
          {
            ConnectionArrowRep* relationRep;
            if( srcEntityRep->second.second == otherRep->second.second )
            {
              relationRep =
                new AutoConnectionArrowRep( srcEntityRep->second.second );
            }
            else
            {
              relationRep = new ConnectionArrowRep( srcEntityRep->second.second,
                otherRep->second.second, false );
            }
            const std::unordered_multimap< shift::EntityGid,
              shift::RelationshipProperties* >& relMMap =
              ( *relatedElements )[ entity->entityGid( ) ];

            auto relMMapIt = relMMap.find( other->entityGid( ));
            if ( relMMapIt != relMMap.end( ))
            {
              auto relProps = relMMapIt->second;
              float weightPropertyValue = 0.0f;
              if ( relProps->hasProperty( "Weight" ))
              {
                weightPropertyValue =
                  relProps->getProperty( "Weight" ).value< float >( );
              }
              else
              {
                Loggers::get( )->log( "Expected property Weight.",
                  LOG_LEVEL_WARNING );
              }

              relationRep->setProperty(
                "width", ( unsigned int ) roundf(
                 _nbConnectionsToWidth.map( fabsf( weightPropertyValue ))));

              // If fixed weight over zero or if gaussian and mean over zero
              // then circle
              relationRep->setProperty(
                "head", shiftgen::ConnectionArrowRep::TArrowHead::TRIANGLE );
              if ( relProps->hasProperty( "Weight Type" ))
              {
                auto weightType = relProps->getProperty( "Weight Type" )
                  .value< shiftgen::ConnectsWith::TFixedOrDistribution >( );
                if ( weightType ==
                  shiftgen::ConnectsWith::TFixedOrDistribution::Fixed
                  && weightPropertyValue < 0.0f )
                {
                  relationRep->setProperty( "head",
                    shiftgen::ConnectionArrowRep::TArrowHead::CIRCLE );
                }
                else if ( weightType
                  == shiftgen::ConnectsWith::TFixedOrDistribution::Gaussian )
                {
                  if ( relProps->hasProperty( "Weight Gaussian" ))
                  {
                    if ( relProps->getProperty( "Weight Gaussian" )
                      .value< float >( ) < 0.0f )
                    {
                      relationRep->setProperty( "head",
                        shiftgen::ConnectionArrowRep::TArrowHead::CIRCLE );
                    }
                  }
                  else
                  {
                    Loggers::get( )->log("Expected property Weight Gaussian.",
                      LOG_LEVEL_WARNING );
                  }
                }
              }
            }

            alreadyConnected = relatedEntitiesReps.insert(
              std::make_pair( combinedKey,
              std::make_tuple( relationRep, entity, other,
              srcEntityRep->second.second, otherRep->second.second )));
          }
          relatedEntities.push_back( std::get< 0 >( alreadyConnected->second ));
        }
      }
    } // generateRelations

    void RepresentationCreator::generateRelations(
      const shift::Entities& entities,
      const shift::TGidToEntitiesReps& gidsToEntitiesReps,
      shift::TRelatedEntitiesReps& relatedEntitiesReps,
      shift::Representations& relatedEntities,
      shift::RelationshipAggregatedOneToN* relatedElements )
    {
      for( auto& entity : entities.vector( ))
      {
        auto srcEntityRep = gidsToEntitiesReps.find( entity->entityGid( ));
        if( srcEntityRep == gidsToEntitiesReps.end( ))
          continue;

        auto entityRelations = relatedElements->mapAggregatedRels( )
          .find( entity->entityGid( ));

        if( entityRelations == relatedElements->mapAggregatedRels( ).end( ))
          continue;

        for( auto& other : entities.vector( ))
        {
          auto otherRep = gidsToEntitiesReps.find( other->entityGid( ));
          if( otherRep == gidsToEntitiesReps.end( ))
            continue;

          auto otherEntityConnection = entityRelations->second
            ->find( other->entityGid( ));
          if( otherEntityConnection == entityRelations->second->end( ))
            continue;

          // TODO: Change to equal_range whenever multiple relationships between
          // the same elements are imported. Then, create a loop to iterate
          // over the given results and create a new one if not found.
          auto combinedKey = std::make_pair( entity->entityGid( ),
                                             other->entityGid( ));
          auto alreadyConnected =
            relatedEntitiesReps.find( combinedKey );

          if( alreadyConnected == relatedEntitiesReps.end( ))
          {
            ConnectionArrowRep* relationRep;
            if( srcEntityRep->second.second == otherRep->second.second )
            {
              relationRep =
                new AutoConnectionArrowRep( srcEntityRep->second.second );
            }
            else
            {
              relationRep = new ConnectionArrowRep(
                srcEntityRep->second.second, otherRep->second.second, true );
            }

            shift::RelationshipProperties* relationProperties =
              otherEntityConnection->second
              .relationshipAggregatedProperties.get( );
            if ( relationProperties )
            {
              float weightPropertyValue = 0.0f;
              if ( relationProperties->hasProperty( "Weight mean" ))
              {
                weightPropertyValue = relationProperties
                  ->getProperty( "Weight mean" ).value< float >( );
              }
              else
              {
                Loggers::get( )->log("Expected property Weight mean.",
                  LOG_LEVEL_WARNING );
              }
              relationRep->setProperty(
                "width", ( unsigned int ) roundf(
                _nbConnectionsToWidth.map( fabsf( weightPropertyValue ))));

              // If fixed weight over zero or if gaussian and mean over zero
              // then circle
              if ( weightPropertyValue < 0.0f  || ( relationProperties
                ->hasProperty( "Weight Gaussian Mean mean" )
                &&  relationProperties->getProperty(
                "Weight Gaussian Mean mean" ).value< float >( ) < 0.0f ))
              {
                relationRep->setProperty(
                  "head", shiftgen::ConnectionArrowRep::TArrowHead::CIRCLE );
              }
              else
              {
                relationRep->setProperty(
                  "head", shiftgen::ConnectionArrowRep::TArrowHead::TRIANGLE );
              }
            }

            alreadyConnected = relatedEntitiesReps.insert(
              std::make_pair( combinedKey,
                std::make_tuple( relationRep,
                entity, other, srcEntityRep->second.second,
                otherRep->second.second )));
          }
          relatedEntities.push_back( std::get< 0 >( alreadyConnected->second ));
        }
      }
    } // generateRelations


    bool RepresentationCreator::entityUpdatedOrCreated( const shift::Entity* entity )
    {
      bool updatedValues = false;
      unsigned int newNeuronsPerPopulation = 0;
      if ( dynamic_cast< const shiftgen::NeuronPop* >( entity )
        || dynamic_cast< const shiftgen::Input* >( entity ))
      {
        if( entity->hasProperty( "Nb of neurons" ))
        {
          newNeuronsPerPopulation =
            entity->getProperty( "Nb of neurons" ).value< unsigned int >( );
        }
        else
        {
          Loggers::get( )->log("Expected property Nb of neurons.",
            LOG_LEVEL_WARNING );
        }
      }
      else if ( dynamic_cast< const shiftgen::NeuronSuperPop* >( entity ))
      {
        if(entity->hasProperty( "Nb of neurons Mean"))
        {
          newNeuronsPerPopulation =
            entity->getProperty( "Nb of neurons Mean" ).value< unsigned int >( );
        }
        else
        {
          Loggers::get( )->log("Expected property Nb of neurons Mean.",
            LOG_LEVEL_WARNING );
        }
        if( entity->hasProperty( "child depth"))
        {
          unsigned  int newLevel =
            entity->getProperty( "child depth" ).value< unsigned int >( );
          if( newLevel > _maxLevelsPerSuperPop )
          {
            this->clear( );
            maxLevelsPerSuperPop( newLevel, false );
            updatedValues = true;
          }
        }
        else
        {
          Loggers::get( )->log("Expected property Nb of neurons Mean.",
            LOG_LEVEL_WARNING );
        }
      }
      if ( newNeuronsPerPopulation > _maxNeuronsPerPopulation )
      {
        this->clear( );
        maxNeuronsPerPopulation( newNeuronsPerPopulation, false );
        updatedValues =  true;
      }
      return updatedValues;
    }

    bool RepresentationCreator::relationshipUpdatedOrCreated(
      const shift::RelationshipProperties* relProperties )
    {
      float newAbsoluteWeight = 0.0f;
      if ( dynamic_cast< const shiftgen::ConnectsWith* >( relProperties ))
      {
        if( relProperties->hasProperty( "Weight Type" )
          &&  relProperties->hasProperty( "Weight" )
          && relProperties->hasProperty( "Weight Gaussian Mean" ))
        {
          newAbsoluteWeight = fabsf( ( relProperties
            ->getProperty( "Weight Type" )
            .value< shiftgen::ConnectsWith::TFixedOrDistribution >( ) ==
            shiftgen::ConnectsWith::TFixedOrDistribution::Fixed ?
            relProperties->getProperty( "Weight" ).value< float >( ) :
            relProperties->getProperty( "Weight Gaussian Mean" )
            .value< float >( )));
        }
        else
        {
          Loggers::get( )->log("Expected properties in connects with.",
            LOG_LEVEL_WARNING );
        }
      }
      else if ( dynamic_cast
        < const shiftgen::AggregatedConnectsWith* >( relProperties ))
      {
        if( relProperties->hasProperty( "Weight mean" ))
        {
          newAbsoluteWeight = fabsf( (
            relProperties->getProperty( "Weight mean" ).value< float >( )));
        }
        else
        {
          Loggers::get( )->log("Expected properies in connects with.",
            LOG_LEVEL_WARNING );
        }
      }
      if( newAbsoluteWeight > _maxAbsoluteWeight )
      {
        maxAbsoluteWeight( newAbsoluteWeight );
        return true;
      }
      else
      {
        return false;
      }
    }

    void RepresentationCreator::maxAbsoluteWeight(
      float maxAbsoluteWeight_, bool compare )
    {
      if ( !compare || maxAbsoluteWeight_ > _maxAbsoluteWeight )
      {
        _maxAbsoluteWeight = maxAbsoluteWeight_;
        _nbConnectionsToWidth =
          MapperFloatToFloat( 0, _maxAbsoluteWeight, 1.0f, 5.0f );
      }
    }

    void RepresentationCreator::maxNeuronsPerPopulation(
      unsigned int maxNeuronsPerPopulation_, bool compare )
    {
      if ( !compare || maxNeuronsPerPopulation_ > _maxNeuronsPerPopulation )
      {
        _maxNeuronsPerPopulation = maxNeuronsPerPopulation_;
        _neuronsToPercentage =
          MapperFloatToFloat ( 0, _maxNeuronsPerPopulation, 0.0f, 1.0f );
      }
    }

    void RepresentationCreator::maxLevelsPerSuperPop(
        unsigned int maxLevelsPerSuperPop_, bool compare )
    {
      if ( !compare || maxLevelsPerSuperPop_ > _maxLevelsPerSuperPop )
      {
        _maxLevelsPerSuperPop = maxLevelsPerSuperPop_;
        _superPopSeparation = 1.0f / ( _maxLevelsPerSuperPop + 1 );
        _superPopLevelSeparation = NeuronSuperPopItem::rangeCircle
          * _superPopSeparation;
          ;
      }
    }

    float RepresentationCreator::maxAbsoluteWeight( void ) const
    {
      return _maxAbsoluteWeight;
    }

    unsigned int RepresentationCreator::maxNeuronsPerPopulation( void ) const
    {
      return _maxNeuronsPerPopulation;
    }

    unsigned int RepresentationCreator::maxLevelsPerSuperPop( void ) const
    {
      return _maxLevelsPerSuperPop;
    }



  } // namespace congen
} // namespace nslib
