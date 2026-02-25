/***************************************************
 * ScriptProvider.hpp
 * Created on Tue, 24 Feb 2026 21:16:22 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <boost/asio/awaitable.hpp>
#include <memory>

#include "Script.hpp"
#include "ScriptKey.hpp"

class ScriptProvider
{
public:
  virtual ~ScriptProvider() = default;
  virtual boost::asio::awaitable<std::shared_ptr<Script>> asyncGet(
    const ScriptKey& key) = 0;
};
