/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MTROPOLIS_RUNTIME_H
#define MTROPOLIS_RUNTIME_H

#include "common/array.h"
#include "common/platform.h"
#include "common/ptr.h"
#include "common/stream.h"
#include "common/hashmap.h"
#include "common/hash-str.h"

#include "mtropolis/data.h"
#include "mtropolis/vthread.h"

namespace MTropolis {

class Project;
class PlugIn;
class Modifier;
struct IModifierFactory;
struct IPlugInModifierFactory;
struct IPlugInModifierFactoryAndDataFactory;
struct ModifierLoaderContext;

struct Point16 {
	int16 x;
	int16 y;

	bool load(const Data::Point &point);

	inline bool operator==(const Point16& other) const {
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const Point16 &other) const {
		return !((*this) == other);
	}
};

struct Rect16 {
	int16 top;
	int16 left;
	int16 bottom;
	int16 right;

	bool load(const Data::Rect &rect);

	inline bool operator==(const Rect16 &other) const {
		return top == other.top && left == other.left && bottom == other.bottom && right == other.right;
	}

	inline bool operator!=(const Rect16 &other) const {
		return !((*this) == other);
	}
};

struct IntRange {
	int32 min;
	int32 max;

	bool load(const Data::IntRange &range);

	inline bool operator==(const IntRange &other) const {
		return min == other.min && max == other.max;
	}

	inline bool operator!=(const IntRange &other) const {
		return !((*this) == other);
	}
};

struct Label {
	uint32 superGroupID;
	uint32 id;

	bool load(const Data::Label &label);

	inline bool operator==(const Label &other) const {
		return superGroupID == other.superGroupID && id == other.id;
	}

	inline bool operator!=(const Label &other) const {
		return !((*this) == other);
	}
};

struct Event {
	uint32 eventType;
	uint32 eventInfo;

	bool load(const Data::Event &data);

	inline bool operator==(const Event &other) const {
		return eventType == other.eventType && eventInfo == other.eventInfo;
	}

	inline bool operator!=(const Event &other) const {
		return !((*this) == other);
	}
};

struct VarReference {
	uint32 guid;
	Common::String *source;

	inline bool operator==(const VarReference &other) const {
		return guid == other.guid && (*source) == (*other.source);
	}

	inline bool operator!=(const VarReference &other) const {
		return !((*this) == other);
	}
};

struct AngleMagVector {
	double angleRadians;
	double magnitude;

	inline bool operator==(const AngleMagVector &other) const {
		return angleRadians == other.angleRadians && magnitude == other.magnitude;
	}

	inline bool operator!=(const AngleMagVector &other) const {
		return !((*this) == other);
	}
};

struct ColorRGB8 {
	uint8 r;
	uint8 g;
	uint8 b;

	bool load(const Data::ColorRGB16 &color);
};

struct MessageFlags {
	MessageFlags();

	bool relay : 1;
	bool cascade : 1;
	bool immediate : 1;
};

namespace DynamicValueTypes {
	enum DynamicValueType {
		kInvalid,

		kNull,
		kInteger,
		kFloat,
		kPoint,
		kIntegerRange,
		kBoolean,
		kVector,
		kLabel,
		kEvent,
		kVariableReference,
		kIncomingData,
		kString,
		kList,

		kEmpty,
	};
}

struct DynamicValue;
struct DynamicList;

class DynamicListContainerBase {
public:
	virtual ~DynamicListContainerBase();
	virtual bool setAtIndex(size_t index, const DynamicValue &dynValue) = 0;
	virtual void setFrom(const DynamicListContainerBase &other) = 0;	// Only supports setting same type!
	virtual const void *getConstArrayPtr() const = 0;
	virtual size_t getSize() const = 0;
	virtual bool compareEqual(const DynamicListContainerBase &other) const = 0;
};

struct DynamicListDefaultSetter {
	static void defaultSet(int32 &value);
	static void defaultSet(double &value);
	static void defaultSet(Point16 &value);
	static void defaultSet(IntRange &value);
	static void defaultSet(bool &value);
	static void defaultSet(AngleMagVector &value);
	static void defaultSet(Label &value);
	static void defaultSet(Event &value);
	static void defaultSet(Common::String &value);
	static void defaultSet(DynamicList &value);
};

struct DynamicListValueImporter {
	static bool importValue(const DynamicValue &dynValue, const int32 *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const double *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const Point16 *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const IntRange *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const bool *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const AngleMagVector *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const Label *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const Event *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const Common::String *&outPtr);
	static bool importValue(const DynamicValue &dynValue, const DynamicList *&outPtr);
};

template<class T>
class DynamicListContainer : public DynamicListContainerBase {
public:
	bool setAtIndex(size_t index, const DynamicValue &dynValue) override;
	void setFrom(const DynamicListContainerBase &other) override;
	const void *getConstArrayPtr() const override;
	size_t getSize() const override;
	bool compareEqual(const DynamicListContainerBase &other) const override;

private:
	Common::Array<T> _array;
};

template<>
class DynamicListContainer<void> : public DynamicListContainerBase {
public:
	DynamicListContainer();

	bool setAtIndex(size_t index, const DynamicValue &dynValue) override;
	void setFrom(const DynamicListContainerBase &other) override;
	const void *getConstArrayPtr() const override;
	size_t getSize() const override;
	bool compareEqual(const DynamicListContainerBase &other) const override;

public:
	size_t _size;
};

template<>
class DynamicListContainer<VarReference> : public DynamicListContainerBase {
public:
	bool setAtIndex(size_t index, const DynamicValue &dynValue) override;
	void setFrom(const DynamicListContainerBase &other) override;
	const void *getConstArrayPtr() const override;
	size_t getSize() const override;
	bool compareEqual(const DynamicListContainerBase &other) const override;

private:
	void rebuildStringPointers();

	Common::Array<VarReference> _array;
	Common::Array<Common::String> _strings;
};

template<class T>
bool DynamicListContainer<T>::setAtIndex(size_t index, const DynamicValue &dynValue) {
	const T *valuePtr = nullptr;
	if (!DynamicListValueImporter::importValue(dynValue, valuePtr))
		return false;

	_array.reserve(index + 1);
	if (_array.size() <= index) {
		if (_array.size() < index) {
			T defaultValue;
			DynamicListDefaultSetter::defaultSet(defaultValue);
			while (_array.size() < index) {
				_array.push_back(defaultValue);
			}
		}
		_array.push_back(*valuePtr);
	} else {
		_array[index] = *valuePtr;
	}

	return true;
}

template<class T>
void DynamicListContainer<T>::setFrom(const DynamicListContainerBase &other) {
	_array = static_cast<const DynamicListContainer<T> &>(other)._array;
}

template<class T>
const void *DynamicListContainer<T>::getConstArrayPtr() const {
	return &_array;
}

template<class T>
size_t DynamicListContainer<T>::getSize() const {
	return _array.size();
}

template<class T>
bool DynamicListContainer<T>::compareEqual(const DynamicListContainerBase &other) const {
	const DynamicListContainer<T> &otherTyped = static_cast<const DynamicListContainer<T> &>(other);
	return _array == otherTyped._array;
}

struct DynamicList {
	DynamicList();
	DynamicList(const DynamicList &other);
	~DynamicList();

	DynamicValueTypes::DynamicValueType getType() const;

	const Common::Array<int32> &getInt() const;
	const Common::Array<double> &getFloat() const;
	const Common::Array<Point16> &getPoint() const;
	const Common::Array<IntRange> &getIntRange() const;
	const Common::Array<AngleMagVector> &getVector() const;
	const Common::Array<Label> &getLabel() const;
	const Common::Array<Event> &getEvent() const;
	const Common::Array<VarReference> &getVarReference() const;
	const Common::Array<Common::String> &getString() const;
	const Common::Array<bool> &getBool() const;

	bool setAtIndex(size_t index, const DynamicValue &value);

	DynamicList &operator=(const DynamicList &other);

	bool operator==(const DynamicList &other) const;
	inline bool operator!=(const DynamicList &other) const {
		return !((*this) == other);
	}

	void swap(DynamicList &other);

private:
	void clear();
	void initFromOther(const DynamicList &other);
	bool changeToType(DynamicValueTypes::DynamicValueType type);

	DynamicValueTypes::DynamicValueType _type;
	DynamicListContainerBase *_container;
};


struct DynamicValue {
	DynamicValue();
	DynamicValue(const DynamicValue &other);
	~DynamicValue();

	bool load(const Data::InternalTypeTaggedValue &data, const Common::String &varSource, const Common::String &varString);
	bool load(const Data::PlugInTypeTaggedValue &data);

	DynamicValueTypes::DynamicValueType getType() const;

	const int32 &getInt() const;
	const double &getFloat() const;
	const Point16 &getPoint() const;
	const IntRange &getIntRange() const;
	const AngleMagVector &getVector() const;
	const Label &getLabel() const;
	const Event &getEvent() const;
	const VarReference &getVarReference() const;
	const Common::String &getString() const;
	const bool &getBool() const;
	const DynamicList &getList() const;

	DynamicValue &operator=(const DynamicValue &other);

	bool operator==(const DynamicValue &other) const;
	inline bool operator!=(const DynamicValue& other) const {
		return !((*this) == other);
	}

	void swap(DynamicValue &other);

private:
	union ValueUnion {
		double asFloat;
		int32 asInt;
		IntRange asIntRange;
		AngleMagVector asVector;
		Label asLabel;
		VarReference asVarReference;
		Event asEvent;
		Point16 asPoint;
		bool asBool;
		DynamicList *asList;
	};

	void clear();
	void initFromOther(const DynamicValue &other);

	DynamicValueTypes::DynamicValueType _type;
	ValueUnion _value;
	Common::String _str;
};

struct MessengerSendSpec {
	MessengerSendSpec();
	bool load(const Data::Event &dataEvent, uint32 dataMessageFlags, const Data::InternalTypeTaggedValue &dataLocator, const Common::String &dataWithSource, const Common::String &dataWithString, uint32 dataDestination);
	bool load(const Data::PlugInTypeTaggedValue &dataEvent, const MessageFlags &dataMessageFlags, const Data::PlugInTypeTaggedValue &dataWith, uint32 dataDestination);

	Event send;
	MessageFlags messageFlags;
	DynamicValue with;
	uint32 destination; // May be a MessageDestination or GUID
};

enum MessageDestination {
	kMessageDestSharedScene = 0x65,
	kMessageDestScene = 0x66,
	kMessageDestSection = 0x67,
	kMessageDestProject = 0x68,
	kMessageDestActiveScene = 0x69,
	kMessageDestElementsParent = 0x6a,
	kMessageDestChildren = 0x6b,
	kMessageDestModifiersParent = 0x6c,
	kMessageDestSubsection = 0x6d,

	kMessageDestElement = 0xc9,
	kMessageDestSourcesParent = 0xcf,

	kMessageDestBehavior = 0xd4,
	kMessageDestNextElement = 0xd1,
	kMessageDestPrevElement = 0xd2,
	kMessageDestBehaviorsParent = 0xd3,
};

struct SegmentDescription {
	int volumeID;
	Common::String filePath;
	Common::SeekableReadStream *stream;
};

struct IPlugInModifierRegistrar {
	virtual void registerPlugInModifier(const char *name, const Data::IPlugInModifierDataFactory *loader, const IPlugInModifierFactory *factory) = 0;
	void registerPlugInModifier(const char *name, const IPlugInModifierFactoryAndDataFactory *loaderFactory);
};

class PlugIn {
public:
	virtual ~PlugIn();

	virtual void registerModifiers(IPlugInModifierRegistrar *registrar) const = 0;
};

struct ProjectResources {
	virtual ~ProjectResources();
};

class ProjectDescription {

public:
	ProjectDescription(); 
	~ProjectDescription();

	void addSegment(int volumeID, const char *filePath);
	void addSegment(int volumeID, Common::SeekableReadStream *stream);
	const Common::Array<SegmentDescription> &getSegments() const;

	void addPlugIn(const Common::SharedPtr<PlugIn> &plugIn);
	const Common::Array<Common::SharedPtr<PlugIn> > &getPlugIns() const;

	void setResources(const Common::SharedPtr<ProjectResources> &resources);
	const Common::SharedPtr<ProjectResources> &getResources() const;

private:
	Common::Array<SegmentDescription> _segments;
	Common::Array<Common::SharedPtr<PlugIn> > _plugIns;
	Common::SharedPtr<ProjectResources> _resources;
};

struct VolumeState {
	Common::String name;
	int volumeID;
	bool isMounted;
};

class Runtime {
public:
	Runtime();

	void runFrame();
	void queueProject(const Common::SharedPtr<ProjectDescription> &desc);

	void addVolume(int volumeID, const char *name, bool isMounted);

private:
	Common::Array<VolumeState> _volumes;
	Common::SharedPtr<ProjectDescription> _queuedProjectDesc;
	Common::SharedPtr<Project> _project;
	Common::ScopedPtr<VThread> _vthread;
};

struct IModifierContainer {
	virtual const Common::Array<Common::SharedPtr<Modifier> > &getModifiers() const = 0;
	virtual void appendModifier(const Common::SharedPtr<Modifier> &modifier) = 0;
};

class SimpleModifierContainer : public IModifierContainer {

	const Common::Array<Common::SharedPtr<Modifier> > &getModifiers() const;
	void appendModifier(const Common::SharedPtr<Modifier> &modifier) override;

private:
	Common::Array<Common::SharedPtr<Modifier> > _modifiers;
};

class Structural : public IModifierContainer {

public:
	virtual ~Structural();

	const Common::Array<Common::SharedPtr<Structural> > &getChildren() const;
	void addChild(const Common::SharedPtr<Structural> &child);

	const Common::Array<Common::SharedPtr<Modifier> > &getModifiers() const override;
	void appendModifier(const Common::SharedPtr<Modifier> &modifier) override;

private:
	Common::Array<Common::SharedPtr<Structural> > _children;
	Common::Array<Common::SharedPtr<Modifier> > _modifiers;
};

struct ProjectPresentationSettings {
	ProjectPresentationSettings();

	uint16 width;
	uint16 height;
	uint32 bitsPerPixel;
};

class AssetInfo {

public:
	AssetInfo();
	virtual ~AssetInfo();

	void setSegmentIndex(int segmentIndex);
	int getSegmentIndex() const;

private:
	int _segmentIndex;
};

struct ChildLoaderContext {
	enum Type {
		kTypeUnknown,
		kTypeModifierList,
		kTypeProject,
		kTypeSection,
		kTypeSubsection,
		kTypeSceneRoots,
	};

	union ContainerUnion {
		IModifierContainer *modifierContainer;
		Structural *structural;
	};

	uint remainingCount;
	Type type;
	ContainerUnion containerUnion;
};

struct ChildLoaderStack {
	Common::Array<ChildLoaderContext> contexts;
};

class ProjectPlugInRegistry : public IPlugInModifierRegistrar  {
public:
	ProjectPlugInRegistry();

	void registerPlugInModifier(const char *name, const Data::IPlugInModifierDataFactory *dataFactory, const IPlugInModifierFactory *factory) override;

	const Data::PlugInModifierRegistry &getDataLoaderRegistry() const;
	const IPlugInModifierFactory *findPlugInModifierFactory(const char *name) const;

private:
	Data::PlugInModifierRegistry _dataLoaderRegistry;
	Common::HashMap<Common::String, const IPlugInModifierFactory *> _factoryRegistry;
};

class Project : public Structural {

public:
	Project();
	~Project();

	void loadFromDescription(const ProjectDescription &desc);

private:
	struct LabelSuperGroup {
		size_t firstRootNodeIndex;
		size_t numRootNodes;
		size_t numTotalNodes;

		uint32 superGroupID;
		Common::String name;
	};

	struct LabelTree {
		size_t firstChildIndex;
		size_t numChildren;

		uint32 id;
		Common::String name;
	};

	struct Segment {
		SegmentDescription desc;
		Common::SharedPtr<Common::SeekableReadStream> rcStream;
		Common::SeekableReadStream *weakStream;
	};

	enum StreamType {
		kStreamTypeUnknown,

		kStreamTypeAsset,
		kStreamTypeBoot,
		kStreamTypeScene,
	};

	struct StreamDesc {
		StreamType streamType;
		uint16 segmentIndex;
		uint32 size;
		uint32 pos;
	};

	struct AssetDesc {
		uint32 typeCode;
		size_t id;
		Common::String name;

		// If the asset is live, this will be its asset info
		Common::SharedPtr<AssetInfo> assetInfo;
	};

	void openSegmentStream(int segmentIndex);
	void loadBootStream(size_t streamIndex);

	void loadPresentationSettings(const Data::PresentationSettings &presentationSettings);
	void loadAssetCatalog(const Data::AssetCatalog &assetCatalog);
	void loadGlobalObjectInfo(ChildLoaderStack &loaderStack, const Data::GlobalObjectInfo &globalObjectInfo);
	void loadContextualObject(ChildLoaderStack &stack, const Data::DataObject &dataObject);
	Common::SharedPtr<Modifier> loadModifierObject(ModifierLoaderContext &loaderContext, const Data::DataObject &dataObject);
	void loadLabelMap(const Data::ProjectLabelMap &projectLabelMap);
	static size_t recursiveCountLabels(const Data::ProjectLabelMap::LabelTree &tree);

	Common::Array<Segment> _segments;
	Common::Array<StreamDesc> _streams;
	Common::Array<LabelTree> _labelTree;
	Common::Array<LabelSuperGroup> _labelSuperGroups;
	Data::ProjectFormat _projectFormat;
	bool _isBigEndian;

	Common::Array<AssetDesc *> _assetsByID;
	Common::Array<AssetDesc> _realAssets;

	Common::HashMap<Common::String, size_t> _assetNameToID;

	ProjectPresentationSettings _presentationSettings;

	bool _haveGlobalObjectInfo;
	bool _haveProjectStructuralDef;
	SimpleModifierContainer _globalModifiers;

	ProjectPlugInRegistry _plugInRegistry;

	Common::Array<Common::SharedPtr<PlugIn> > _plugIns;
	Common::SharedPtr<ProjectResources> _resources;
};

class Section : public Structural {
};

class Subsection : public Structural {
};

class Scene : public Structural {
};

struct ModifierFlags {
	ModifierFlags();
	bool load(const uint32 dataModifierFlags);

	bool isLastModifier : 1;
};

class Modifier {
public:
	Modifier();
	virtual ~Modifier();

protected:
	uint32 _guid;
	Common::String _name;
	ModifierFlags _modifierFlags;

	bool loadTypicalHeader(const Data::TypicalModifierHeader &typicalHeader);
};

} // End of namespace MTropolis

#endif
