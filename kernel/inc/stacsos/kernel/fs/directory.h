#pragma once

#include <stacsos/kernel/fs/fs-node.h>
#include <stacsos/kernel/debug.h>

namespace stacsos::kernel::fs {

class directory {

public:

	directory(fs_node &node)
		: fsNode_(node)
	{
	}

	virtual ~directory() {}

	fs_node &get_node() { return fsNode_; }

private:
	fs_node &fsNode_;
};

}
