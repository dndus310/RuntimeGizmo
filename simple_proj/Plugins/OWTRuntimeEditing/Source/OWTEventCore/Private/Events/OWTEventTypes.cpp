#include "Events/OWTEventTypes.h"

FOWTEventRecord::FOWTEventRecord()
    : TimestampUtc(), Event(NAME_None), Json(), Recipient(), Direction(EOWTEventDirection::Outbound), Sequence(0),
      bValidJson(false), bPayloadTruncated(false)
{
}
