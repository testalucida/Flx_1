#ifndef _FLX_HELPER_H
#define _FLX_HELPER_H

class Fl_Group;

namespace flx {
	class Flx_Helper {
	public:
		static Flx_Helper &instance();
		/** print properties of grp and its children */
		void dump( const Fl_Group &grp, bool recursive = false ) const;
		static void destroy();
	private:
		Flx_Helper();
	private:
		static Flx_Helper *_pThis;
	};
}

#endif