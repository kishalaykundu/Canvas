/**
 * @file Types.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Enum classes used by the Canvas simulate system. Types.h is
 * automatically generated by the generateEnums program based on
 * input in Assets/Config/EnumTypes.xml
 */

#pragma once

#include <iostream>
#include <string>

using std::ostream;
using std::string;

namespace Sim {

	 enum class ManagerType {
		EventManager,
		RenderManager,
		ComputeManager,
		TaskManager,
		PluginManager,
		AssetManager,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const ManagerType& id)
	 {
		 switch (id){
		 case ManagerType::EventManager: output << "EventManager"; break;
		 case ManagerType::RenderManager: output << "RenderManager"; break;
		 case ManagerType::ComputeManager: output << "ComputeManager"; break;
		 case ManagerType::TaskManager: output << "TaskManager"; break;
		 case ManagerType::PluginManager: output << "PluginManager"; break;
		 case ManagerType::AssetManager: output << "AssetManager"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto ManagerTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("EventManager")){
			 return ManagerType::EventManager;
		 }
		 else if (!str.compare ("RenderManager")){
			 return ManagerType::RenderManager;
		 }
		 else if (!str.compare ("ComputeManager")){
			 return ManagerType::ComputeManager;
		 }
		 else if (!str.compare ("TaskManager")){
			 return ManagerType::TaskManager;
		 }
		 else if (!str.compare ("PluginManager")){
			 return ManagerType::PluginManager;
		 }
		 else if (!str.compare ("AssetManager")){
			 return ManagerType::AssetManager;
		 }
		 return ManagerType::Unknown;
	 };

	 enum class RenderManagerType {
		GLManager,
		VKManager,
		DXManager,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const RenderManagerType& id)
	 {
		 switch (id){
		 case RenderManagerType::GLManager: output << "GLManager"; break;
		 case RenderManagerType::VKManager: output << "VKManager"; break;
		 case RenderManagerType::DXManager: output << "DXManager"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto RenderManagerTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("GLManager")){
			 return RenderManagerType::GLManager;
		 }
		 else if (!str.compare ("VKManager")){
			 return RenderManagerType::VKManager;
		 }
		 else if (!str.compare ("DXManager")){
			 return RenderManagerType::DXManager;
		 }
		 return RenderManagerType::Unknown;
	 };

	 enum class ComputeManagerType {
		CudaManager,
		CLManager,
		ComputeManager,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const ComputeManagerType& id)
	 {
		 switch (id){
		 case ComputeManagerType::CudaManager: output << "CudaManager"; break;
		 case ComputeManagerType::CLManager: output << "CLManager"; break;
		 case ComputeManagerType::ComputeManager: output << "ComputeManager"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto ComputeManagerTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("CudaManager")){
			 return ComputeManagerType::CudaManager;
		 }
		 else if (!str.compare ("CLManager")){
			 return ComputeManagerType::CLManager;
		 }
		 else if (!str.compare ("ComputeManager")){
			 return ComputeManagerType::ComputeManager;
		 }
		 return ComputeManagerType::Unknown;
	 };

	 enum class TaskManagerType {
		TbbManager,
		ThreadManager,
		TaskManager,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const TaskManagerType& id)
	 {
		 switch (id){
		 case TaskManagerType::TbbManager: output << "TbbManager"; break;
		 case TaskManagerType::ThreadManager: output << "ThreadManager"; break;
		 case TaskManagerType::TaskManager: output << "TaskManager"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto TaskManagerTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("TbbManager")){
			 return TaskManagerType::TbbManager;
		 }
		 else if (!str.compare ("ThreadManager")){
			 return TaskManagerType::ThreadManager;
		 }
		 else if (!str.compare ("TaskManager")){
			 return TaskManagerType::TaskManager;
		 }
		 return TaskManagerType::Unknown;
	 };

	 enum class PluginType {
		Rigid,
		CpuMsd,
		CudaMsd,
		OclMsd,
		ComputeMsd,
		CpuXfem,
		CudaXfem,
		OclXfem,
		ComputeXfem,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const PluginType& id)
	 {
		 switch (id){
		 case PluginType::Rigid: output << "Rigid"; break;
		 case PluginType::CpuMsd: output << "CpuMsd"; break;
		 case PluginType::CudaMsd: output << "CudaMsd"; break;
		 case PluginType::OclMsd: output << "OclMsd"; break;
		 case PluginType::ComputeMsd: output << "ComputeMsd"; break;
		 case PluginType::CpuXfem: output << "CpuXfem"; break;
		 case PluginType::CudaXfem: output << "CudaXfem"; break;
		 case PluginType::OclXfem: output << "OclXfem"; break;
		 case PluginType::ComputeXfem: output << "ComputeXfem"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto PluginTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("Rigid")){
			 return PluginType::Rigid;
		 }
		 else if (!str.compare ("CpuMsd")){
			 return PluginType::CpuMsd;
		 }
		 else if (!str.compare ("CudaMsd")){
			 return PluginType::CudaMsd;
		 }
		 else if (!str.compare ("OclMsd")){
			 return PluginType::OclMsd;
		 }
		 else if (!str.compare ("ComputeMsd")){
			 return PluginType::ComputeMsd;
		 }
		 else if (!str.compare ("CpuXfem")){
			 return PluginType::CpuXfem;
		 }
		 else if (!str.compare ("CudaXfem")){
			 return PluginType::CudaXfem;
		 }
		 else if (!str.compare ("OclXfem")){
			 return PluginType::OclXfem;
		 }
		 else if (!str.compare ("ComputeXfem")){
			 return PluginType::ComputeXfem;
		 }
		 return PluginType::Unknown;
	 };

	 enum class AssetId {
		LeftKidney,
		RightKidney,
		GallBladder,
		Liver,
		Scalpel,
		Retractor,
		Apple,
		Melon,
		Cube,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const AssetId& id)
	 {
		 switch (id){
		 case AssetId::LeftKidney: output << "LeftKidney"; break;
		 case AssetId::RightKidney: output << "RightKidney"; break;
		 case AssetId::GallBladder: output << "GallBladder"; break;
		 case AssetId::Liver: output << "Liver"; break;
		 case AssetId::Scalpel: output << "Scalpel"; break;
		 case AssetId::Retractor: output << "Retractor"; break;
		 case AssetId::Apple: output << "Apple"; break;
		 case AssetId::Melon: output << "Melon"; break;
		 case AssetId::Cube: output << "Cube"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto AssetIdByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("LeftKidney")){
			 return AssetId::LeftKidney;
		 }
		 else if (!str.compare ("RightKidney")){
			 return AssetId::RightKidney;
		 }
		 else if (!str.compare ("GallBladder")){
			 return AssetId::GallBladder;
		 }
		 else if (!str.compare ("Liver")){
			 return AssetId::Liver;
		 }
		 else if (!str.compare ("Scalpel")){
			 return AssetId::Scalpel;
		 }
		 else if (!str.compare ("Retractor")){
			 return AssetId::Retractor;
		 }
		 else if (!str.compare ("Apple")){
			 return AssetId::Apple;
		 }
		 else if (!str.compare ("Melon")){
			 return AssetId::Melon;
		 }
		 else if (!str.compare ("Cube")){
			 return AssetId::Cube;
		 }
		 return AssetId::Unknown;
	 };

	 enum class AssetType {
		Rigid,
		Deformable_XFEM,
		Deformable_MSD,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const AssetType& id)
	 {
		 switch (id){
		 case AssetType::Rigid: output << "Rigid"; break;
		 case AssetType::Deformable_XFEM: output << "Deformable_XFEM"; break;
		 case AssetType::Deformable_MSD: output << "Deformable_MSD"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto AssetTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("Rigid")){
			 return AssetType::Rigid;
		 }
		 else if (!str.compare ("Deformable_XFEM")){
			 return AssetType::Deformable_XFEM;
		 }
		 else if (!str.compare ("Deformable_MSD")){
			 return AssetType::Deformable_MSD;
		 }
		 return AssetType::Unknown;
	 };

	 enum class AssetComponentType {
		Geometry,
		Render,
		Physics,
		Collision,
		Intersection,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const AssetComponentType& id)
	 {
		 switch (id){
		 case AssetComponentType::Geometry: output << "Geometry"; break;
		 case AssetComponentType::Render: output << "Render"; break;
		 case AssetComponentType::Physics: output << "Physics"; break;
		 case AssetComponentType::Collision: output << "Collision"; break;
		 case AssetComponentType::Intersection: output << "Intersection"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto AssetComponentTypeByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("Geometry")){
			 return AssetComponentType::Geometry;
		 }
		 else if (!str.compare ("Render")){
			 return AssetComponentType::Render;
		 }
		 else if (!str.compare ("Physics")){
			 return AssetComponentType::Physics;
		 }
		 else if (!str.compare ("Collision")){
			 return AssetComponentType::Collision;
		 }
		 else if (!str.compare ("Intersection")){
			 return AssetComponentType::Intersection;
		 }
		 return AssetComponentType::Unknown;
	 };

	 enum class ProgramId {
		Normal,
		MSD,
		XfeSurface,
		XfeCut,
		Unknown
	 };

	 inline ostream& operator << (ostream& output, const ProgramId& id)
	 {
		 switch (id){
		 case ProgramId::Normal: output << "Normal"; break;
		 case ProgramId::MSD: output << "MSD"; break;
		 case ProgramId::XfeSurface: output << "XfeSurface"; break;
		 case ProgramId::XfeCut: output << "XfeCut"; break;
		 default: output << "Unknown"; break;
		 }
		 return output;
	 }

	 auto ProgramIdByName = [=] (const char* chr)
	 {
		 string str (chr);
		 if (!str.compare ("Normal")){
			 return ProgramId::Normal;
		 }
		 else if (!str.compare ("MSD")){
			 return ProgramId::MSD;
		 }
		 else if (!str.compare ("XfeSurface")){
			 return ProgramId::XfeSurface;
		 }
		 else if (!str.compare ("XfeCut")){
			 return ProgramId::XfeCut;
		 }
		 return ProgramId::Unknown;
	 };

}
