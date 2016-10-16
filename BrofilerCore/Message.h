#pragma once
#include "Common.h"
#include "Serialization.h"

namespace Brofiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const uint32 NETWORK_PROTOCOL_VERSION = 9;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataResponse
{
	enum Type
	{
		FrameDescriptionBoard,			// DescriptionBoard for Instrumental Frames
		EventFrame,						// Instrumental Data
		SamplingFrame,					// Sampling Data
		Synchronization,				// SwitchContext Data
		NullFrame,						// Last Fame Mark
		ReportProgress,					// Report Current Progress
		Handshake,						// Handshake Response
	};

	uint32 version;
	uint32 size;
	Type type;

	DataResponse(Type t, uint32 s) : version(NETWORK_PROTOCOL_VERSION), size(s), type(t) {}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator << (OutputDataStream& os, const DataResponse& val);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IMessage
{
public:
	enum Type
	{
		Start,
		Stop,
		TurnSampling,
		COUNT,
	};

	virtual void Apply() = 0;
	virtual ~IMessage() {}

	static IMessage* Create( InputDataStream& str );
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IMessage::Type MESSAGE_TYPE>
class Message : public IMessage
{
	enum { id = MESSAGE_TYPE };
public:
	static uint32 GetMessageType() { return id; }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StartMessage : public Message<IMessage::Start>
{
	static IMessage* Create(InputDataStream&);
	virtual void Apply() override;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StopMessage : public Message<IMessage::Stop>
{
	static IMessage* Create(InputDataStream&);
	virtual void Apply() override;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TurnSamplingMessage : public Message<IMessage::TurnSampling>
{
	int32 index;
	byte isSampling;

	static IMessage* Create(InputDataStream& stream);
	virtual void Apply() override;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}