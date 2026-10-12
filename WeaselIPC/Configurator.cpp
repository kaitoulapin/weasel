#include "stdafx.h"
#include "Deserializer.h"
#include "Configurator.h"

using namespace weasel;

Deserializer::Ptr Configurator::Create(ResponseParser* pTarget) {
  return Deserializer::Ptr(new Configurator(pTarget));
}

Configurator::Configurator(ResponseParser* pTarget) : Deserializer(pTarget) {}

Configurator::~Configurator() {}

void Configurator::Store(Deserializer::KeyType const& key,
                         std::wstring const& value) {
  if (key.size() < 2)
    return;
  if (key[1] == L"reserve_mark_space") {
    if (m_pTarget->p_style && (value == L"0" || value == L"1"))
      m_pTarget->p_style->reserve_mark_space = (value == L"1");
    return;
  }
  // inline_preedit is sent first on every response, including by old servers
  // that do not send reserve_mark_space. Missing extensions keep the default.
  if (key[1] == L"inline_preedit" && m_pTarget->p_style)
    m_pTarget->p_style->reserve_mark_space = false;
  if (!m_pTarget->p_context || !m_pTarget->p_config)
    return;
  bool bool_value = (!value.empty() && value != L"0");
  if (key[1] == L"inline_preedit") {
    m_pTarget->p_config->inline_preedit = bool_value;
  }
}
