#include <ios>
#include <array>
#include <boost/endian/conversion.hpp>
#include <BytesBuffer.hpp>
#include <sstream>
#include "CloakedDagger.hpp"

CloakedDagger::CloakedDagger(std::istream& inputStream):
  magic1(read<std::uint32_t>(inputStream)),
  majorVersion(read<std::uint16_t>(inputStream)),
  minorVersion(read<std::uint16_t>(inputStream)),
  headerLength(read<std::uint32_t>(inputStream)),
  encapsulationType(read<std::uint32_t>(inputStream)),
  encapsulationConfig(read<std::uint16_t>(inputStream)),
  encapsulationDataLength(read<std::uint16_t>(inputStream)),
  key(readArray(inputStream)),
  headerChecksumType(read<std::uint32_t>(inputStream)),
  headerChecksumConfig(read<std::uint16_t>(inputStream)),
  headerChecksumDataLength(read<std::uint16_t>(inputStream)),
  dataChecksumType(read<std::uint32_t>(inputStream)),
  dataChecksumDataLength(read<std::uint32_t>(inputStream)),
  magic2(read<std::uint32_t>(inputStream))
{
  throwIfHeaderInvalid();
}

void CloakedDagger::throwIfHeaderInvalid() const
{
  if ((boost::endian::native_to_big(magic1) != 0xd1df5fff) ||
      (boost::endian::native_to_big(magic2) != 0xff5fdfd1) ||
      (boost::endian::native_to_big(majorVersion) != 1) ||
      (boost::endian::native_to_big(minorVersion) != 0) ||
      (boost::endian::native_to_big(encapsulationType) != 1) ||
      (boost::endian::native_to_big(encapsulationConfig) != 3) ||
      (boost::endian::native_to_big(encapsulationDataLength) != maskLength))
  {
    throw std::runtime_error("The header did not decode correctly");
  }
}

CloakedDagger CloakedDagger::createFromBuffer(const BytesBuffer& buffer)
{
  if (buffer.size() < CloakedDagger::headerSize())
  {
    throw std::runtime_error("Insufficient space in file for header: " + std::to_string(buffer.size()) );
  }

  std::vector<char> headerBuffer(
    buffer.begin(),
    buffer.begin() + CloakedDagger::headerSize());


  std::stringstream s;
  s.write(headerBuffer.data(), headerBuffer.size());

  return CloakedDagger(s);
}


